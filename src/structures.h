#pragma once
#include <string>
#include <vector>
#include <map>
#include <chrono>
#ifdef _WIN32
#include <windows.h>
#endif

struct Function {
    std::vector<std::string> parameters;
    std::vector<std::string> body;
    int startLine;
};

struct GameObject {
    int x, y, width, height;
    int vx, vy; // velocidad
    std::string color;
    bool visible;
};

struct WindowInfo {
    HWND hwnd;
    HWND textArea;
    std::string title;
    std::string color;
    bool isActive;
    std::wstring content;
    bool isGameWindow;
    std::map<std::string, GameObject> gameObjects;
    bool keysPressed[256];
    std::chrono::steady_clock::time_point lastUpdate;
    bool gameRunning;
};

struct UIElement {
    HWND hwnd;
    std::string type;
    std::string id;
    int x, y, width, height;
};