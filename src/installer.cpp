#include <windows.h>
#include <shlobj.h>
#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <vector>
#include <commctrl.h>

// IDs para controles
#define IDC_INSTALL_BTN 1001
#define IDC_UNINSTALL_BTN 1002
#define IDC_BROWSE_BTN 1003
#define IDC_PATH_EDIT 1004
#define IDC_PROGRESS 1005
#define IDC_STATUS_TEXT 1006
#define IDC_EXIT_BTN 1007
#define IDC_CHECKBOX_PATH 1011
#define IDC_CHECKBOX_ASSOC 1012

// Macros para botones (compatibilidad MinGW)
#ifndef Button_GetCheck
#define Button_GetCheck(hwnd) ((int)(DWORD)SendMessage((hwnd), BM_GETCHECK, 0L, 0L))
#endif
#ifndef Button_SetCheck
#define Button_SetCheck(hwnd, check) ((void)SendMessage((hwnd), BM_SETCHECK, (WPARAM)(int)(check), 0L))
#endif

class SimplePancracioInstaller {
private:
    HWND hMainWnd;
    HWND hPathEdit;
    HWND hProgressBar;
    HWND hStatusText;
    HWND hCheckboxPath;
    HWND hCheckboxAssoc;
    std::wstring installPath;
    bool isInstalled;
    
    std::wstring StringToWString(const std::string& str) {
        if (str.empty()) return std::wstring();
        int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
        std::wstring result(size, 0);
        MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &result[0], size);
        return result;
    }
    
    std::string WStringToString(const std::wstring& wstr) {
        if (wstr.empty()) return std::string();
        int size = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
        std::string result(size, 0);
        WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &result[0], size, NULL, NULL);
        return result;
    }
    
    bool IsRunAsAdmin() {
        BOOL isAdmin = FALSE;
        PSID adminGroup = NULL;
        
        SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;
        if (AllocateAndInitializeSid(&ntAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID,
                                   DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &adminGroup)) {
            CheckTokenMembership(NULL, adminGroup, &isAdmin);
            FreeSid(adminGroup);
        }
        return isAdmin == TRUE;
    }
    
    bool RequestAdminPrivileges() {
        if (IsRunAsAdmin()) return true;
        
        wchar_t exePath[MAX_PATH];
        GetModuleFileNameW(NULL, exePath, MAX_PATH);
        
        // Obtener argumentos de lÃ­nea de comandos
        LPWSTR cmdLine = GetCommandLineW();
        
        SHELLEXECUTEINFOW sei = {0};
        sei.cbSize = sizeof(SHELLEXECUTEINFOW);
        sei.lpVerb = L"runas";
        sei.lpFile = exePath;
        sei.lpParameters = cmdLine;
        sei.hwnd = NULL;
        sei.nShow = SW_NORMAL;
        
        if (ShellExecuteExW(&sei)) {
            // El programa se reiniciÃ³ con permisos de administrador
            ExitProcess(0);
        }
        
        return false;
    }
    
    bool CheckIfInstalled() {
        HKEY hKey;
        LONG result = RegOpenKeyExW(HKEY_LOCAL_MACHINE, 
                                   L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Pancracio",
                                   0, KEY_READ, &hKey);
        if (result == ERROR_SUCCESS) {
            RegCloseKey(hKey);
            return true;
        }
        return false;
    }
    
    void UpdateStatus(const std::wstring& status) {
        SetWindowTextW(hStatusText, status.c_str());
        UpdateWindow(hStatusText);
    }
    
    void UpdateProgress(int percent) {
        SendMessage(hProgressBar, PBM_SETPOS, percent, 0);
        UpdateWindow(hProgressBar);
    }
    
    std::wstring BrowseForFolder() {
        BROWSEINFOW bi = {0};
        bi.hwndOwner = hMainWnd;
        bi.lpszTitle = L"Seleccione la carpeta de instalaciÃ³n:";
        bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_USENEWUI;
        
        LPITEMIDLIST pidl = SHBrowseForFolderW(&bi);
        if (pidl != NULL) {
            wchar_t path[MAX_PATH];
            if (SHGetPathFromIDListW(pidl, path)) {
                CoTaskMemFree(pidl);
                return std::wstring(path);
            }
            CoTaskMemFree(pidl);
        }
        return L"";
    }
    
    bool CreateDirectories() {
        try {
            UpdateStatus(L"Creando directorios...");
            UpdateProgress(20);
            
            std::filesystem::create_directories(installPath + L"\\Pancracio");
            std::filesystem::create_directories(installPath + L"\\Pancracio\\bin");
            std::filesystem::create_directories(installPath + L"\\Pancracio\\logo");
            return true;
        } catch (const std::exception& e) {
            std::wstring error = L"Error creando directorios: " + StringToWString(e.what());
            MessageBoxW(hMainWnd, error.c_str(), L"Error", MB_OK | MB_ICONERROR);
            return false;
        }
    }
    
    bool CopyFiles() {
        try {
            UpdateStatus(L"Copiando archivos...");
            UpdateProgress(40);
            
            // Copiar el ejecutable principal
            std::ifstream src("pan_core.exe", std::ios::binary);
            std::string dstPathStr = WStringToString(installPath + L"\\Pancracio\\bin\\pan.exe");
            std::ofstream dst(dstPathStr, std::ios::binary);
            if (!src || !dst) {
                MessageBoxW(hMainWnd, L"Error: No se encontrÃ³ pan_core.exe", L"Error", MB_OK | MB_ICONERROR);
                return false;
            }
            dst << src.rdbuf();
            src.close();
            dst.close();
            
            UpdateProgress(45);
            
            // Copiar el logo
            std::ifstream logoSrc("logo\\logo.ico", std::ios::binary);
            std::string logoDstPathStr = WStringToString(installPath + L"\\Pancracio\\logo\\logo.ico");
            std::ofstream logoDst(logoDstPathStr, std::ios::binary);
            if (logoSrc && logoDst) {
                logoDst << logoSrc.rdbuf();
            }
            logoSrc.close();
            logoDst.close();
            
            // Copiar el logo2 para archivos .pansos
            std::ifstream logo2Src("logo\\logo2.ico", std::ios::binary);
            std::string logo2DstPathStr = WStringToString(installPath + L"\\Pancracio\\logo\\logo2.ico");
            std::ofstream logo2Dst(logo2DstPathStr, std::ios::binary);
            if (logo2Src && logo2Dst) {
                logo2Dst << logo2Src.rdbuf();
            }
            logo2Src.close();
            logo2Dst.close();
            
            UpdateProgress(50);
            
            // Copiar documentaciÃ³n
            std::ifstream docSrc("documentation.md", std::ios::binary);
            std::string docDstPathStr = WStringToString(installPath + L"\\Pancracio\\documentation.md");
            std::ofstream docDst(docDstPathStr, std::ios::binary);
            if (docSrc && docDst) {
                docDst << docSrc.rdbuf();
            }
            docSrc.close();
            docDst.close();
            
            UpdateProgress(55);
            
            // Crear desinstalador
            CreateUninstaller();
            
            return true;
        } catch (const std::exception& e) {
            std::wstring error = L"Error copiando archivos: " + StringToWString(e.what());
            MessageBoxW(hMainWnd, error.c_str(), L"Error", MB_OK | MB_ICONERROR);
            return false;
        }
    }
    
    void CreateUninstaller() {
        UpdateStatus(L"Creando desinstalador...");
        UpdateProgress(58);
        
        // Copiar el instalador actual como desinstalador
        wchar_t currentPath[MAX_PATH];
        GetModuleFileNameW(NULL, currentPath, MAX_PATH);
        
        std::wstring uninstallerPath = installPath + L"\\Pancracio\\uninstall.exe";
        CopyFileW(currentPath, uninstallerPath.c_str(), FALSE);
    }
    
    bool AddToPath() {
        if (!Button_GetCheck(hCheckboxPath)) return true;
        
        UpdateStatus(L"Agregando al PATH del sistema...");
        UpdateProgress(60);
        
        HKEY hKey;
        LONG result = RegOpenKeyExW(HKEY_LOCAL_MACHINE, 
                                   L"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment",
                                   0, KEY_READ | KEY_WRITE, &hKey);
        
        if (result != ERROR_SUCCESS) {
            MessageBoxW(hMainWnd, L"Error accediendo al registro del sistema", L"Error", MB_OK | MB_ICONERROR);
            return false;
        }
        
        wchar_t currentPath[32768];
        DWORD pathSize = sizeof(currentPath);
        DWORD pathType;
        
        result = RegQueryValueExW(hKey, L"PATH", NULL, &pathType, (LPBYTE)currentPath, &pathSize);
        if (result != ERROR_SUCCESS) {
            RegCloseKey(hKey);
            return false;
        }
        
        std::wstring newPath = std::wstring(currentPath);
        std::wstring binPath = installPath + L"\\Pancracio\\bin";
        
        // Verificar si ya estÃ¡ en el PATH
        if (newPath.find(binPath) == std::wstring::npos) {
            if (!newPath.empty() && newPath.back() != L';') {
                newPath += L";";
            }
            newPath += binPath;
            
            result = RegSetValueExW(hKey, L"PATH", 0, REG_EXPAND_SZ, 
                                   (const BYTE*)newPath.c_str(), (DWORD)((newPath.length() + 1) * sizeof(wchar_t)));
        }
        
        RegCloseKey(hKey);
        
        // Notificar cambios en variables de entorno
        SendMessageTimeoutW(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)L"Environment", 
                           SMTO_ABORTIFHUNG, 5000, NULL);
        
        return result == ERROR_SUCCESS;
    }
    
    void CreateFileAssociation() {
        if (!Button_GetCheck(hCheckboxAssoc)) return;
        
        UpdateStatus(L"Creando asociaciÃ³n de archivos...");
        UpdateProgress(70);
        
        HKEY hKey;
        
        // Crear asociaciÃ³n para archivos .pan
        RegCreateKeyExW(HKEY_CLASSES_ROOT, L".pan", 0, NULL, REG_OPTION_NON_VOLATILE, 
                        KEY_WRITE, NULL, &hKey, NULL);
        RegSetValueExW(hKey, L"", 0, REG_SZ, (const BYTE*)L"PancracioFile", 26);
        RegCloseKey(hKey);
        
        // Crear asociación para archivos .pansos
        RegCreateKeyExW(HKEY_CLASSES_ROOT, L".pansos", 0, NULL, REG_OPTION_NON_VOLATILE, 
                        KEY_WRITE, NULL, &hKey, NULL);
        RegSetValueExW(hKey, L"", 0, REG_SZ, (const BYTE*)L"PancracioDataFile", 36);
        RegCloseKey(hKey);
        
        // Crear tipo de archivo
        RegCreateKeyExW(HKEY_CLASSES_ROOT, L"PancracioFile", 0, NULL, REG_OPTION_NON_VOLATILE, 
                        KEY_WRITE, NULL, &hKey, NULL);
        RegSetValueExW(hKey, L"", 0, REG_SZ, (const BYTE*)L"Archivo Pancracio", 36);
        RegCloseKey(hKey);
        
        // Crear tipo de archivo para .pansos
        RegCreateKeyExW(HKEY_CLASSES_ROOT, L"PancracioDataFile", 0, NULL, REG_OPTION_NON_VOLATILE, 
                        KEY_WRITE, NULL, &hKey, NULL);
        RegSetValueExW(hKey, L"", 0, REG_SZ, (const BYTE*)L"Archivo de Datos Pancracio", 54);
        RegCloseKey(hKey);
        
        // Establecer icono
        std::wstring iconPath = installPath + L"\\Pancracio\\logo\\logo.ico";
        RegCreateKeyExW(HKEY_CLASSES_ROOT, L"PancracioFile\\DefaultIcon", 0, NULL, 
                        REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);
        RegSetValueExW(hKey, L"", 0, REG_SZ, (const BYTE*)iconPath.c_str(), (DWORD)((iconPath.length() + 1) * sizeof(wchar_t)));
        RegCloseKey(hKey);
        
        // Establecer icono para .pansos
        std::wstring icon2Path = installPath + L"\\Pancracio\\logo\\logo2.ico";
        RegCreateKeyExW(HKEY_CLASSES_ROOT, L"PancracioDataFile\\DefaultIcon", 0, NULL, 
                        REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);
        RegSetValueExW(hKey, L"", 0, REG_SZ, (const BYTE*)icon2Path.c_str(), (DWORD)((icon2Path.length() + 1) * sizeof(wchar_t)));
        RegCloseKey(hKey);
        
        // Establecer comando de apertura
        std::wstring command = L"\"" + installPath + L"\\Pancracio\\bin\\pan.exe\" \"%1\"";
        RegCreateKeyExW(HKEY_CLASSES_ROOT, L"PancracioFile\\shell\\open\\command", 0, NULL, 
                        REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);
        RegSetValueExW(hKey, L"", 0, REG_SZ, (const BYTE*)command.c_str(), (DWORD)((command.length() + 1) * sizeof(wchar_t)));
        RegCloseKey(hKey);
        
        // Establecer comando de apertura para .pansos (abrir con notepad)
        std::wstring notepadCommand = L"notepad.exe \"%1\"";
        RegCreateKeyExW(HKEY_CLASSES_ROOT, L"PancracioDataFile\\shell\\open\\command", 0, NULL, 
                        REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);
        RegSetValueExW(hKey, L"", 0, REG_SZ, (const BYTE*)notepadCommand.c_str(), (DWORD)((notepadCommand.length() + 1) * sizeof(wchar_t)));
        RegCloseKey(hKey);
        
        // Refrescar iconos
        SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);
    }
    
    void CreateUninstallEntry() {
        UpdateStatus(L"Registrando desinstalador...");
        UpdateProgress(80);
        
        HKEY hKey;
        RegCreateKeyExW(HKEY_LOCAL_MACHINE, 
                        L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Pancracio",
                        0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);
        
        std::wstring displayName = L"Pancracio Programming Language";
        std::wstring uninstallString = installPath + L"\\Pancracio\\uninstall.exe";
        std::wstring installLocation = installPath + L"\\Pancracio";
        std::wstring version = L"1.0.0";
        std::wstring publisher = L"Pancracio Team";
        
        RegSetValueExW(hKey, L"DisplayName", 0, REG_SZ, 
                      (const BYTE*)displayName.c_str(), (DWORD)((displayName.length() + 1) * sizeof(wchar_t)));
        RegSetValueExW(hKey, L"UninstallString", 0, REG_SZ, 
                      (const BYTE*)uninstallString.c_str(), (DWORD)((uninstallString.length() + 1) * sizeof(wchar_t)));
        RegSetValueExW(hKey, L"InstallLocation", 0, REG_SZ, 
                      (const BYTE*)installLocation.c_str(), (DWORD)((installLocation.length() + 1) * sizeof(wchar_t)));
        RegSetValueExW(hKey, L"DisplayVersion", 0, REG_SZ, 
                      (const BYTE*)version.c_str(), (DWORD)((version.length() + 1) * sizeof(wchar_t)));
        RegSetValueExW(hKey, L"Publisher", 0, REG_SZ, 
                      (const BYTE*)publisher.c_str(), (DWORD)((publisher.length() + 1) * sizeof(wchar_t)));
        
        DWORD noModify = 1;
        RegSetValueExW(hKey, L"NoModify", 0, REG_DWORD, (const BYTE*)&noModify, sizeof(DWORD));
        RegSetValueExW(hKey, L"NoRepair", 0, REG_DWORD, (const BYTE*)&noModify, sizeof(DWORD));
        
        RegCloseKey(hKey);
    }
    
    bool RemoveFromPath() {
        UpdateStatus(L"Removiendo del PATH...");
        UpdateProgress(30);
        
        HKEY hKey;
        LONG result = RegOpenKeyExW(HKEY_LOCAL_MACHINE, 
                                   L"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment",
                                   0, KEY_READ | KEY_WRITE, &hKey);
        
        if (result != ERROR_SUCCESS) return false;
        
        wchar_t currentPath[32768];
        DWORD pathSize = sizeof(currentPath);
        DWORD pathType;
        
        result = RegQueryValueExW(hKey, L"PATH", NULL, &pathType, (LPBYTE)currentPath, &pathSize);
        if (result != ERROR_SUCCESS) {
            RegCloseKey(hKey);
            return false;
        }
        
        std::wstring pathStr = std::wstring(currentPath);
        std::wstring binPath = installPath + L"\\Pancracio\\bin";
        
        // Remover del PATH
        size_t pos = pathStr.find(binPath);
        if (pos != std::wstring::npos) {
            size_t endPos = pos + binPath.length();
            if (endPos < pathStr.length() && pathStr[endPos] == L';') {
                endPos++;
            } else if (pos > 0 && pathStr[pos - 1] == L';') {
                pos--;
            }
            pathStr.erase(pos, endPos - pos);
            
            result = RegSetValueExW(hKey, L"PATH", 0, REG_EXPAND_SZ, 
                                   (const BYTE*)pathStr.c_str(), (DWORD)((pathStr.length() + 1) * sizeof(wchar_t)));
        }
        
        RegCloseKey(hKey);
        
        SendMessageTimeoutW(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)L"Environment", 
                           SMTO_ABORTIFHUNG, 5000, NULL);
        
        return result == ERROR_SUCCESS;
    }
    
    void RemoveFileAssociation() {
        UpdateStatus(L"Removiendo asociaciones de archivos...");
        UpdateProgress(50);
        RegDeleteTreeW(HKEY_CLASSES_ROOT, L".pan");
        RegDeleteTreeW(HKEY_CLASSES_ROOT, L".pansos");
        RegDeleteTreeW(HKEY_CLASSES_ROOT, L"PancracioFile");
        RegDeleteTreeW(HKEY_CLASSES_ROOT, L"PancracioDataFile");
        SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);
    }
    
    void RemoveUninstallEntry() {
        UpdateProgress(60);
        RegDeleteKeyW(HKEY_LOCAL_MACHINE, 
                     L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Pancracio");
    }
    
    bool DoInstall() {
        if (!CreateDirectories()) return false;
        if (!CopyFiles()) return false;
        if (!AddToPath()) return false;
        CreateFileAssociation();
        CreateUninstallEntry();
        
        UpdateStatus(L"Â¡InstalaciÃ³n completada!");
        UpdateProgress(100);
        
        std::wstring message = L"Â¡Pancracio se ha instalado correctamente!\n\n"
                              L"UbicaciÃ³n: " + installPath + L"\\Pancracio\n\n"
                              L"Para desinstalar use Panel de Control o ejecute uninstall.exe";
        MessageBoxW(hMainWnd, message.c_str(),
                   L"InstalaciÃ³n Completada", MB_OK | MB_ICONINFORMATION);
        
        return true;
    }
    
    bool DoUninstall() {
        UpdateStatus(L"Desinstalando Pancracio...");
        UpdateProgress(10);
        
        // Obtener ruta de instalaciÃ³n del registro
        HKEY hKey;
        if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, 
                         L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Pancracio",
                         0, KEY_READ, &hKey) == ERROR_SUCCESS) {
            wchar_t installLoc[MAX_PATH];
            DWORD size = sizeof(installLoc);
            if (RegQueryValueExW(hKey, L"InstallLocation", NULL, NULL, (LPBYTE)installLoc, &size) == ERROR_SUCCESS) {
                installPath = std::wstring(installLoc);
                size_t pos = installPath.find_last_of(L"\\");
                if (pos != std::wstring::npos) {
                    installPath = installPath.substr(0, pos);
                }
            }
            RegCloseKey(hKey);
        }
        
        UpdateProgress(20);
        
        // Remover del PATH del sistema
        RemoveFromPath();
        
        // Remover asociaciones de archivos
        RemoveFileAssociation();
        
        // Remover entrada del registro
        RemoveUninstallEntry();
        
        UpdateStatus(L"Eliminando archivos...");
        UpdateProgress(70);
        
        // Crear script batch para eliminar todo despuÃ©s de cerrar el programa
        std::wstring batchPath = installPath + L"\\Pancracio\\cleanup.bat";
        std::ofstream batchFile(WStringToString(batchPath));
        if (batchFile.is_open()) {
            batchFile << "@echo off\n";
            batchFile << "timeout /t 2 /nobreak >nul\n";
            batchFile << "rmdir /s /q \"" << WStringToString(installPath + L"\\Pancracio") << "\"\n";
            batchFile << "del \"%~f0\"\n";
            batchFile.close();
            
            // Ejecutar el script batch de forma oculta
            STARTUPINFOA si = {0};
            PROCESS_INFORMATION pi = {0};
            si.cb = sizeof(si);
            si.dwFlags = STARTF_USESHOWWINDOW;
            si.wShowWindow = SW_HIDE;
            
            std::string batchCmd = "cmd.exe /c \"" + WStringToString(batchPath) + "\"";
            CreateProcessA(NULL, (LPSTR)batchCmd.c_str(), NULL, NULL, FALSE, 
                          CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
            
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
        
        UpdateStatus(L"Â¡DesinstalaciÃ³n completada!");
        UpdateProgress(100);
        
        MessageBoxW(hMainWnd, 
                   L"Pancracio ha sido desinstalado correctamente.\n\n"
                   L"Todos los archivos y configuraciones han sido eliminados.",
                   L"DesinstalaciÃ³n Completada", MB_OK | MB_ICONINFORMATION);
        
        return true;
    }
    
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        SimplePancracioInstaller* installer = (SimplePancracioInstaller*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        
        switch (uMsg) {
            case WM_COMMAND: {
                switch (LOWORD(wParam)) {
                    case IDC_BROWSE_BTN: {
                        std::wstring newPath = installer->BrowseForFolder();
                        if (!newPath.empty()) {
                            installer->installPath = newPath;
                            SetWindowTextW(installer->hPathEdit, installer->installPath.c_str());
                        }
                        break;
                    }
                    
                    case IDC_INSTALL_BTN: {
                        wchar_t pathBuffer[MAX_PATH];
                        GetWindowTextW(installer->hPathEdit, pathBuffer, MAX_PATH);
                        installer->installPath = std::wstring(pathBuffer);
                        
                        EnableWindow(GetDlgItem(hwnd, IDC_INSTALL_BTN), FALSE);
                        EnableWindow(GetDlgItem(hwnd, IDC_UNINSTALL_BTN), FALSE);
                        EnableWindow(GetDlgItem(hwnd, IDC_BROWSE_BTN), FALSE);
                        
                        if (installer->DoInstall()) {
                            installer->isInstalled = true;
                            EnableWindow(GetDlgItem(hwnd, IDC_UNINSTALL_BTN), TRUE);
                        } else {
                            EnableWindow(GetDlgItem(hwnd, IDC_INSTALL_BTN), TRUE);
                            EnableWindow(GetDlgItem(hwnd, IDC_BROWSE_BTN), TRUE);
                        }
                        break;
                    }
                    
                    case IDC_UNINSTALL_BTN: {
                        int result = MessageBoxW(hwnd, 
                                                L"Â¿EstÃ¡ seguro de que desea desinstalar Pancracio?",
                                                L"Confirmar DesinstalaciÃ³n", 
                                                MB_YESNO | MB_ICONQUESTION);
                        
                        if (result == IDYES) {
                            EnableWindow(GetDlgItem(hwnd, IDC_INSTALL_BTN), FALSE);
                            EnableWindow(GetDlgItem(hwnd, IDC_UNINSTALL_BTN), FALSE);
                            
                            if (installer->DoUninstall()) {
                                installer->isInstalled = false;
                                EnableWindow(GetDlgItem(hwnd, IDC_INSTALL_BTN), TRUE);
                                EnableWindow(GetDlgItem(hwnd, IDC_BROWSE_BTN), TRUE);
                                // Cerrar el programa despuÃ©s de desinstalar
                                PostQuitMessage(0);
                            } else {
                                EnableWindow(GetDlgItem(hwnd, IDC_UNINSTALL_BTN), TRUE);
                            }
                        }
                        break;
                    }
                    
                    case IDC_EXIT_BTN:
                        PostQuitMessage(0);
                        break;
                }
                break;
            }
            
            case WM_DESTROY:
                PostQuitMessage(0);
                break;
                
            default:
                return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
        return 0;
    }
    
public:
    SimplePancracioInstaller() {
        installPath = L"C:";
        isInstalled = false;
    }
    
    int Run() {
        // Configurar UTF-8 para tildes
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
        
        // Verificar argumentos de lÃ­nea de comandos
        int argc;
        LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
        
        if (argc > 1) {
            if (wcscmp(argv[1], L"--uninstall") == 0 || wcscmp(argv[1], L"--uninstall-silent") == 0) {
                // Solicitar permisos de administrador si no los tiene
                if (!RequestAdminPrivileges()) {
                    MessageBoxW(NULL, L"Se requieren permisos de administrador para desinstalar.", 
                               L"Permisos Requeridos", MB_OK | MB_ICONWARNING);
                    return 1;
                }
                
                isInstalled = CheckIfInstalled();
                if (isInstalled) {
                    DoUninstall();
                }
                return 0;
            }
        }
        
        // Solicitar permisos de administrador automÃ¡ticamente
        if (!RequestAdminPrivileges()) {
            MessageBoxW(NULL, 
                       L"Este programa requiere permisos de administrador.\n\n"
                       L"No se pudieron obtener los permisos necesarios.",
                       L"Permisos Requeridos", MB_OK | MB_ICONWARNING);
            return 1;
        }
        
        CoInitialize(NULL);
        
        INITCOMMONCONTROLSEX icex;
        icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
        icex.dwICC = ICC_PROGRESS_CLASS | ICC_STANDARD_CLASSES;
        InitCommonControlsEx(&icex);
        
        isInstalled = CheckIfInstalled();
        
        // Registrar clase de ventana
        WNDCLASSEXW wc = {0};
        wc.cbSize = sizeof(WNDCLASSEXW);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = GetModuleHandle(NULL);
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wc.lpszClassName = L"SimplePancracioInstaller";
        wc.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(1));
        wc.hIconSm = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(1));
        
        RegisterClassExW(&wc);
        
        // Crear ventana principal simple
        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);
        int windowWidth = 500;
        int windowHeight = 350;
        int x = (screenWidth - windowWidth) / 2;
        int y = (screenHeight - windowHeight) / 2;
        
        hMainWnd = CreateWindowExW(
            0,
            L"SimplePancracioInstaller",
            L"Instalador Pancracio",
            WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
            x, y, windowWidth, windowHeight,
            NULL, NULL, GetModuleHandle(NULL), NULL
        );
        
        SetWindowLongPtr(hMainWnd, GWLP_USERDATA, (LONG_PTR)this);
        
        // Crear controles simples
        CreateWindowW(L"STATIC", L"Pancracio Programming Language", 
                     WS_VISIBLE | WS_CHILD | SS_CENTER,
                     50, 20, 400, 25, hMainWnd, NULL, GetModuleHandle(NULL), NULL);
        
        CreateWindowW(L"STATIC", L"Ruta de instalaciÃ³n:", 
                     WS_VISIBLE | WS_CHILD,
                     50, 60, 150, 20, hMainWnd, NULL, GetModuleHandle(NULL), NULL);
        
        hPathEdit = CreateWindowW(L"EDIT", installPath.c_str(),
                                 WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
                                 50, 85, 300, 25, hMainWnd, (HMENU)IDC_PATH_EDIT, 
                                 GetModuleHandle(NULL), NULL);
        
        CreateWindowW(L"BUTTON", L"Examinar",
                     WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                     360, 85, 80, 25, hMainWnd, (HMENU)IDC_BROWSE_BTN, 
                     GetModuleHandle(NULL), NULL);
        
        hCheckboxPath = CreateWindowW(L"BUTTON", L"Agregar al PATH del sistema",
                                     WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
                                     50, 125, 250, 20, hMainWnd, (HMENU)IDC_CHECKBOX_PATH, 
                                     GetModuleHandle(NULL), NULL);
        
        hCheckboxAssoc = CreateWindowW(L"BUTTON", L"Asociar archivos .pan",
                                      WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
                                      50, 150, 300, 20, hMainWnd, (HMENU)IDC_CHECKBOX_ASSOC, 
                                      GetModuleHandle(NULL), NULL);
        
        // Actualizar texto del checkbox para incluir .pansos
        SetWindowTextW(hCheckboxAssoc, L"Asociar archivos .pan y .pansos");
        
        // Marcar checkboxes por defecto
        Button_SetCheck(hCheckboxPath, BST_CHECKED);
        Button_SetCheck(hCheckboxAssoc, BST_CHECKED);
        
        // Botones principales
        CreateWindowW(L"BUTTON", L"Instalar",
                     WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_DEFPUSHBUTTON,
                     50, 190, 100, 35, hMainWnd, (HMENU)IDC_INSTALL_BTN, 
                     GetModuleHandle(NULL), NULL);
        
        HWND hUninstallBtn = CreateWindowW(L"BUTTON", L"Desinstalar",
                                          WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                                          160, 190, 100, 35, hMainWnd, (HMENU)IDC_UNINSTALL_BTN, 
                                          GetModuleHandle(NULL), NULL);
        
        CreateWindowW(L"BUTTON", L"Salir",
                     WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                     340, 190, 100, 35, hMainWnd, (HMENU)IDC_EXIT_BTN, 
                     GetModuleHandle(NULL), NULL);
        
        // Barra de progreso
        hProgressBar = CreateWindowW(PROGRESS_CLASSW, NULL,
                                    WS_VISIBLE | WS_CHILD,
                                    50, 240, 390, 20, hMainWnd, (HMENU)IDC_PROGRESS, 
                                    GetModuleHandle(NULL), NULL);
        
        // Texto de estado
        hStatusText = CreateWindowW(L"STATIC", L"Listo para instalar",
                                   WS_VISIBLE | WS_CHILD | SS_LEFT,
                                   50, 270, 390, 20, hMainWnd, (HMENU)IDC_STATUS_TEXT, 
                                   GetModuleHandle(NULL), NULL);
        
        // Configurar estado inicial
        EnableWindow(hUninstallBtn, isInstalled);
        SendMessage(hProgressBar, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
        
        ShowWindow(hMainWnd, SW_SHOW);
        UpdateWindow(hMainWnd);
        
        // Bucle de mensajes
        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        
        CoUninitialize();
        return 0;
    }
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    SimplePancracioInstaller installer;
    return installer.Run();
}