#include <iostream>
#include <string>
#include <filesystem>
#ifdef _WIN32
#include <windows.h>
#include <shlobj.h>
#include <io.h>
#include <fcntl.h>
#include <commctrl.h>
#include <richedit.h>
#undef min
#undef max
#endif

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")

#include "interpreter.h"

int main(int argc, char* argv[]) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    
    if (argc == 2 && std::string(argv[1]) == "--uninstall") {
        std::cout << "Iniciando desinstalacion de Pancracio..." << std::endl;
        
        char exePath[MAX_PATH];
        GetModuleFileNameA(NULL, exePath, MAX_PATH);
        std::filesystem::path currentPath(exePath);
        std::filesystem::path installerPath = currentPath.parent_path() / "uninstall.exe";
        
        if (std::filesystem::exists(installerPath)) {
            std::string command = "\"" + installerPath.string() + "\" --uninstall-silent";
            system(command.c_str());
        } else {
            std::cout << "No se encontro el desinstalador. Use Panel de Control para desinstalar." << std::endl;
        }
        return 0;
    }
    
    if (argc == 2 && std::string(argv[1]) == "--version") {
        PancracioInterpreter interpreter;
        interpreter.showVersion();
        return 0;
    }
    
    if (argc != 2) {
        std::cerr << "Uso: pan archivo.pan" << std::endl;
        std::cerr << "  o: pan --uninstall (para desinstalar)" << std::endl;
        std::cerr << "  o: pan --version (mostrar version)" << std::endl;
        return 1;
    }
    
    std::string filename = argv[1];
    if (filename.length() < 4 || filename.substr(filename.length() - 4) != ".pan") {
        std::cerr << "Error: El archivo debe tener extension .pan" << std::endl;
        return 1;
    }
    
    PancracioInterpreter interpreter;
    interpreter.executeFile(filename);
    
    return 0;
}