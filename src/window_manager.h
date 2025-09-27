#pragma once
#include <vector>
#include <map>
#include <string>
#include "structures.h"

class WindowManager {
private:
    std::vector<WindowInfo> windows;
    std::map<std::string, UIElement> uiElements;
    static WindowManager* instance;

    void drawGameWindow(HDC hdc, WindowInfo& win);
    void updateGameLogic(WindowInfo& win);
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
    WindowManager();
    ~WindowManager();
    
    void createWindow(const std::string& title, const std::string& colorName, bool isGame = false);
    void writeToWindow(const std::string& windowTitle, const std::string& text);
    void runMessageLoop();
    
    void handleKeyDown(HWND hwnd, int keyCode);
    void handleKeyUp(HWND hwnd, int keyCode);
    void handleButtonClick(int buttonId);
    
    std::vector<WindowInfo>& getWindows() { return windows; }
    static void setInstance(WindowManager* instance);
};