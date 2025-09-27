#include "window_manager.h"
#include "utils.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>
#ifdef _WIN32
#include <windows.h>
#include <commctrl.h>
#include <richedit.h>
#endif

WindowManager* WindowManager::instance = nullptr;

WindowManager::WindowManager() {
    instance = this;
}

WindowManager::~WindowManager() {
    instance = nullptr;
}

void WindowManager::setInstance(WindowManager* inst) {
    instance = inst;
}

LRESULT CALLBACK WindowManager::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            return 0;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            
            // Find window info for this hwnd
            if (instance) {
                for (auto& win : instance->windows) {
                    if (win.hwnd == hwnd) {
                        if (win.isGameWindow) {
                            // Draw game objects
                            instance->drawGameWindow(hdc, win);
                        } else {
                            // Regular window background
                            LONG_PTR userData = GetWindowLongPtr(hwnd, GWLP_USERDATA);
                            if (userData) {
                                COLORREF* color = reinterpret_cast<COLORREF*>(userData);
                                HBRUSH brush = CreateSolidBrush(*color);
                                FillRect(hdc, &ps.rcPaint, brush);
                                DeleteObject(brush);
                            }
                        }
                        break;
                    }
                }
            }
            
            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_KEYDOWN: {
            if (instance) {
                instance->handleKeyDown(hwnd, static_cast<int>(wParam));
            }
            return 0;
        }
        case WM_KEYUP: {
            if (instance) {
                instance->handleKeyUp(hwnd, static_cast<int>(wParam));
            }
            return 0;
        }
        case WM_COMMAND: {
            if (HIWORD(wParam) == BN_CLICKED) {
                int buttonId = LOWORD(wParam);
                if (instance) {
                    instance->handleButtonClick(buttonId);
                }
            }
            return 0;
        }
        case WM_SIZE: {
            RECT rect;
            GetClientRect(hwnd, &rect);
            
            if (instance) {
                for (auto& win : instance->windows) {
                    if (win.hwnd == hwnd && win.textArea && !win.isGameWindow) {
                        SetWindowPos(win.textArea, NULL, 10, 10, 
                                   rect.right - 20, rect.bottom - 60, 
                                   SWP_NOZORDER);
                        break;
                    }
                }
            }
            return 0;
        }
        case WM_DESTROY: {
            if (instance) {
                auto it = std::find_if(instance->windows.begin(), instance->windows.end(),
                    [hwnd](const WindowInfo& w) { return w.hwnd == hwnd; });
                if (it != instance->windows.end()) {
                    instance->windows.erase(it);
                }
            }
            
            if (instance && instance->windows.empty()) {
                PostQuitMessage(0);
            }
            return 0;
        }
        case WM_CLOSE: {
            DestroyWindow(hwnd);
            return 0;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void WindowManager::drawGameWindow(HDC hdc, WindowInfo& win) {
    RECT rect;
    GetClientRect(win.hwnd, &rect);
    
    // Clear background
    HBRUSH bgBrush = CreateSolidBrush(RGB(0, 0, 0));
    FillRect(hdc, &rect, bgBrush);
    DeleteObject(bgBrush);
    
    // Draw game objects
    for (auto& [name, obj] : win.gameObjects) {
        if (obj.visible) {
            COLORREF objColor = Utils::getColorFromName(obj.color);
            HBRUSH objBrush = CreateSolidBrush(objColor);
            RECT objRect = {obj.x, obj.y, obj.x + obj.width, obj.y + obj.height};
            FillRect(hdc, &objRect, objBrush);
            DeleteObject(objBrush);
        }
    }
}

void WindowManager::handleKeyDown(HWND hwnd, int keyCode) {
    for (auto& win : windows) {
        if (win.hwnd == hwnd && win.isGameWindow) {
            win.keysPressed[keyCode] = true;
            break;
        }
    }
}

void WindowManager::handleKeyUp(HWND hwnd, int keyCode) {
    for (auto& win : windows) {
        if (win.hwnd == hwnd && win.isGameWindow) {
            win.keysPressed[keyCode] = false;
            break;
        }
    }
}

void WindowManager::handleButtonClick(int buttonId) {
    std::cout << "BotÃ³n presionado: " << buttonId << std::endl;
}

void WindowManager::createWindow(const std::string& title, const std::string& colorName, bool isGame) {
    static bool classRegistered = false;
    const wchar_t* CLASS_NAME = L"PancracioWindow";
    
    if (!classRegistered) {
        INITCOMMONCONTROLSEX icex;
        icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
        icex.dwICC = ICC_WIN95_CLASSES;
        InitCommonControlsEx(&icex);
        
        WNDCLASSW wc = {};
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = GetModuleHandle(NULL);
        wc.lpszClassName = CLASS_NAME;
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        
        RegisterClassW(&wc);
        classRegistered = true;
    }
    
    COLORREF* bgColor = new COLORREF(Utils::getColorFromName(colorName));
    
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, title.c_str(), -1, NULL, 0);
    std::wstring wtitle(size_needed, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, title.c_str(), -1, &wtitle[0], size_needed);
    wtitle.resize(size_needed - 1);
    
    HWND hwnd = CreateWindowExW(
        0,
        CLASS_NAME,
        wtitle.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL, NULL, GetModuleHandle(NULL), NULL
    );
    
    if (hwnd) {
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(bgColor));
        
        WindowInfo winInfo;
        winInfo.hwnd = hwnd;
        winInfo.textArea = nullptr;
        winInfo.title = title;
        winInfo.color = colorName;
        winInfo.isActive = true;
        winInfo.content = L"";
        winInfo.isGameWindow = isGame;
        winInfo.gameRunning = false;
        winInfo.lastUpdate = std::chrono::steady_clock::now();
        
        // Initialize key states
        for (int i = 0; i < 256; i++) {
            winInfo.keysPressed[i] = false;
        }
        
        if (!isGame) {
            // Create text area for regular windows
            HWND textArea = CreateWindowExW(
                WS_EX_CLIENTEDGE,
                L"EDIT",
                L"",
                WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_WANTRETURN,
                10, 10, 760, 500,
                hwnd, (HMENU)1001, GetModuleHandle(NULL), NULL
            );
            
            HFONT hFont = CreateFontW(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                                    DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                    DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
            SendMessage(textArea, WM_SETFONT, (WPARAM)hFont, TRUE);
            
            winInfo.textArea = textArea;
            SetFocus(textArea);
        }
        
        ShowWindow(hwnd, SW_SHOW);
        UpdateWindow(hwnd);
        
        windows.push_back(winInfo);
        
        std::cout << "Ventana '" << title << "' creada exitosamente" << std::endl;
    } else {
        std::cerr << "Error: No se pudo crear la ventana" << std::endl;
        delete bgColor;
    }
}

void WindowManager::writeToWindow(const std::string& windowTitle, const std::string& text) {
    for (auto& win : windows) {
        if (win.title == windowTitle && win.textArea && !win.isGameWindow) {
            int size_needed = MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, NULL, 0);
            std::wstring wtext(size_needed, L'\0');
            MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, &wtext[0], size_needed);
            wtext.resize(size_needed - 1);
            
            int currentLength = GetWindowTextLengthW(win.textArea);
            SendMessageW(win.textArea, EM_SETSEL, currentLength, currentLength);
            SendMessageW(win.textArea, EM_REPLACESEL, FALSE, (LPARAM)wtext.c_str());
            
            if (!text.empty() && text.back() != '\n') {
                SendMessageW(win.textArea, EM_REPLACESEL, FALSE, (LPARAM)L"\r\n");
            }
            
            SendMessageW(win.textArea, EM_SCROLLCARET, 0, 0);
            
            win.content += wtext + L"\r\n";
            return;
        }
    }
    std::cerr << "Error: Ventana '" << windowTitle << "' no encontrada" << std::endl;
}

void WindowManager::runMessageLoop() {
    MSG msg;
    auto lastGameUpdate = std::chrono::steady_clock::now();
    
    while (!windows.empty()) {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                return;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        
        // Update game logic for game windows
        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastGameUpdate).count() >= 16) {
            for (auto& win : windows) {
                if (win.isGameWindow && win.gameRunning) {
                    updateGameLogic(win);
                    InvalidateRect(win.hwnd, NULL, FALSE);
                }
            }
            lastGameUpdate = now;
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void WindowManager::updateGameLogic(WindowInfo& win) {
    RECT rect;
    GetClientRect(win.hwnd, &rect);
    
    for (auto& [name, obj] : win.gameObjects) {
        if (obj.visible) {
            obj.x += obj.vx;
            obj.y += obj.vy;
            
            // Basic boundary collision
            if (obj.x <= 0 || obj.x + obj.width >= rect.right) {
                obj.vx = -obj.vx;
                obj.x = std::max(0, std::min(obj.x, static_cast<int>(rect.right) - obj.width));
            }
            if (obj.y <= 0 || obj.y + obj.height >= rect.bottom) {
                obj.vy = -obj.vy;
                obj.y = std::max(0, std::min(obj.y, static_cast<int>(rect.bottom) - obj.height));
            }
        }
    }
}