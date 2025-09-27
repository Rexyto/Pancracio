#include "command_executor.h"
#include "utils.h"
#include <iostream>
#include <limits>
#include <thread>
#include <chrono>

CommandExecutor::CommandExecutor() {
    windowManager = new WindowManager();
    bookManager = new BookManager();
    dependencyManager = new DependencyManager();
    evaluator = new ExpressionEvaluator(intVariables, stringVariables, floatVariables, 
                                       arrayVariables, windowManager->getWindows());
}

CommandExecutor::~CommandExecutor() {
    delete windowManager;
    delete bookManager;
    delete dependencyManager;
    delete evaluator;
}

std::string CommandExecutor::handleInputFunction(const std::string& funcCall) {
    if (funcCall.find("pedir_numero(") == 0) {
        size_t start = funcCall.find('(') + 1;
        size_t end = funcCall.find_last_of(')');
        
        if (start < end) {
            std::string message = Utils::trim(funcCall.substr(start, end - start));
            if (!message.empty()) {
                std::string prompt = evaluator->evaluateStringExpression(message);
                std::cout << prompt;
            } else {
                std::cout << "Ingrese un numero: ";
            }
        } else {
            std::cout << "Ingrese un numero: ";
        }
        
        int num;
        while (!(std::cin >> num)) {
            std::cout << "Error: Ingrese un numero valido: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        std::cin.ignore();
        return std::to_string(num);
    }
    
    if (funcCall.find("pedir_nombre(") == 0) {
        size_t start = funcCall.find('(') + 1;
        size_t end = funcCall.find_last_of(')');
        
        if (start < end) {
            std::string message = Utils::trim(funcCall.substr(start, end - start));
            if (!message.empty()) {
                std::string prompt = evaluator->evaluateStringExpression(message);
                std::cout << prompt;
            } else {
                std::cout << "Ingrese un texto: ";
            }
        } else {
            std::cout << "Ingrese un texto: ";
        }
        
        std::string nombre;
        std::getline(std::cin, nombre);
        return nombre;
    }
    return "";
}

int CommandExecutor::findMatchingListo(const std::vector<std::string>& lines, int startLine) {
    int depth = 1;
    for (size_t i = startLine + 1; i < lines.size(); i++) {
        std::string line = Utils::trim(lines[i]);
        if (line.find("probar ") == 0 || line.find("receta ") == 0) {
            depth++;
        } else if (line == "listo") {
            depth--;
            if (depth == 0) {
                return static_cast<int>(i);
            }
        }
    }
    return -1;
}

void CommandExecutor::executeHornear(const std::string& args) {
    std::string output = evaluator->evaluateStringExpression(args);
    std::cout << output << std::endl;
}

void CommandExecutor::executeMostrador(const std::string& args) {
    std::vector<std::pair<std::string, std::string>> parts = Utils::splitExpression(args);
    
    std::string title = "Ventana Pancracio";
    std::string color = "blanco";
    
    if (parts.size() >= 1) {
        title = evaluator->evaluateStringExpression(parts[0].first);
    }
    if (parts.size() >= 2) {
        color = evaluator->evaluateStringExpression(parts[1].first);
    }
    
    windowManager->createWindow(title, color);
}

void CommandExecutor::executeJuego(const std::string& args) {
    std::vector<std::pair<std::string, std::string>> parts = Utils::splitExpression(args);
    
    std::string title = "Juego Pancracio";
    
    if (parts.size() >= 1) {
        title = evaluator->evaluateStringExpression(parts[0].first);
    }
    
    windowManager->createWindow(title, "negro", true);
    
    for (auto& win : windowManager->getWindows()) {
        if (win.title == title && win.isGameWindow) {
            win.gameRunning = true;
            break;
        }
    }
}

void CommandExecutor::executeObjeto(const std::string& args) {
    std::vector<std::pair<std::string, std::string>> parts = Utils::splitExpression(args);
    
    if (parts.size() < 6) {
        std::cerr << "Error: objeto necesita al menos 6 parametros" << std::endl;
        return;
    }
    
    std::string windowTitle = evaluator->evaluateStringExpression(parts[0].first);
    std::string objName = evaluator->evaluateStringExpression(parts[1].first);
    int x = evaluator->evaluateIntExpression(parts[2].first);
    int y = evaluator->evaluateIntExpression(parts[3].first);
    int width = evaluator->evaluateIntExpression(parts[4].first);
    int height = evaluator->evaluateIntExpression(parts[5].first);
    std::string color = parts.size() >= 7 ? evaluator->evaluateStringExpression(parts[6].first) : "blanco";
    
    for (auto& win : windowManager->getWindows()) {
        if (win.title == windowTitle && win.isGameWindow) {
            GameObject obj;
            obj.x = x; obj.y = y; obj.width = width; obj.height = height;
            obj.vx = 0; obj.vy = 0; obj.color = color; obj.visible = true;
            win.gameObjects[objName] = obj;
            InvalidateRect(win.hwnd, NULL, FALSE);
            return;
        }
    }
    std::cerr << "Error: Ventana de juego '" << windowTitle << "' no encontrada" << std::endl;
}

void CommandExecutor::executeMover(const std::string& args) {
    std::vector<std::pair<std::string, std::string>> parts = Utils::splitExpression(args);
    
    if (parts.size() < 4) {
        std::cerr << "Error: mover necesita al menos 4 parametros" << std::endl;
        return;
    }
    
    std::string windowTitle = evaluator->evaluateStringExpression(parts[0].first);
    std::string objName = evaluator->evaluateStringExpression(parts[1].first);
    int vx = evaluator->evaluateIntExpression(parts[2].first);
    int vy = evaluator->evaluateIntExpression(parts[3].first);
    
    for (auto& win : windowManager->getWindows()) {
        if (win.title == windowTitle && win.isGameWindow) {
            auto it = win.gameObjects.find(objName);
            if (it != win.gameObjects.end()) {
                it->second.vx = vx; it->second.vy = vy;
                return;
            }
        }
    }
    std::cerr << "Error: Objeto '" << objName << "' no encontrado" << std::endl;
}

void CommandExecutor::executeColision(const std::string& args, bool& result) {
    std::vector<std::pair<std::string, std::string>> parts = Utils::splitExpression(args);
    
    if (parts.size() < 3) {
        result = false;
        return;
    }
    
    std::string windowTitle = evaluator->evaluateStringExpression(parts[0].first);
    std::string obj1Name = evaluator->evaluateStringExpression(parts[1].first);
    std::string obj2Name = evaluator->evaluateStringExpression(parts[2].first);
    
    for (auto& win : windowManager->getWindows()) {
        if (win.title == windowTitle && win.isGameWindow) {
            auto it1 = win.gameObjects.find(obj1Name);
            auto it2 = win.gameObjects.find(obj2Name);
            
            if (it1 != win.gameObjects.end() && it2 != win.gameObjects.end()) {
                GameObject& obj1 = it1->second;
                GameObject& obj2 = it2->second;
                
                if (obj1.visible && obj2.visible) {
                    result = (obj1.x < obj2.x + obj2.width &&
                             obj1.x + obj1.width > obj2.x &&
                             obj1.y < obj2.y + obj2.height &&
                             obj1.y + obj1.height > obj2.y);
                    return;
                }
            }
        }
    }
    result = false;
}

void CommandExecutor::executeOcultar(const std::string& args) {
    std::vector<std::pair<std::string, std::string>> parts = Utils::splitExpression(args);
    
    if (parts.size() < 2) {
        std::cerr << "Error: ocultar necesita 2 parametros" << std::endl;
        return;
    }
    
    std::string windowTitle = evaluator->evaluateStringExpression(parts[0].first);
    std::string objName = evaluator->evaluateStringExpression(parts[1].first);
    
    for (auto& win : windowManager->getWindows()) {
        if (win.title == windowTitle && win.isGameWindow) {
            auto it = win.gameObjects.find(objName);
            if (it != win.gameObjects.end()) {
                it->second.visible = false;
                InvalidateRect(win.hwnd, NULL, FALSE);
                return;
            }
        }
    }
    std::cerr << "Error: Objeto '" << objName << "' no encontrado" << std::endl;
}

void CommandExecutor::executePosicionar(const std::string& args) {
    std::vector<std::pair<std::string, std::string>> parts = Utils::splitExpression(args);
    
    if (parts.size() < 4) {
        std::cerr << "Error: posicionar necesita 4 parametros" << std::endl;
        return;
    }
    
    std::string windowTitle = evaluator->evaluateStringExpression(parts[0].first);
    std::string objName = evaluator->evaluateStringExpression(parts[1].first);
    int x = evaluator->evaluateIntExpression(parts[2].first);
    int y = evaluator->evaluateIntExpression(parts[3].first);
    
    for (auto& win : windowManager->getWindows()) {
        if (win.title == windowTitle && win.isGameWindow) {
            auto it = win.gameObjects.find(objName);
            if (it != win.gameObjects.end()) {
                it->second.x = x; it->second.y = y;
                return;
            }
        }
    }
}

void CommandExecutor::executeEscribir(const std::string& args) {
    std::vector<std::pair<std::string, std::string>> parts = Utils::splitExpression(args);
    
    if (parts.size() < 2) {
        std::cerr << "Error: escribir necesita al menos 2 parametros" << std::endl;
        return;
    }
    
    std::string windowTitle = evaluator->evaluateStringExpression(parts[0].first);
    std::string text = evaluator->evaluateStringExpression(parts[1].first);
    
    windowManager->writeToWindow(windowTitle, text);
}

void CommandExecutor::executeLibro(const std::string& args) {
    std::vector<std::pair<std::string, std::string>> parts = Utils::splitExpression(args);
    
    if (parts.empty()) {
        std::cerr << "Error: libro necesita al menos 1 parametro" << std::endl;
        return;
    }
    
    std::string action = evaluator->evaluateStringExpression(parts[0].first);
    
    if (action == "crear") {
        if (parts.size() < 2) {
            std::cerr << "Error: libro \"crear\" necesita el nombre del archivo" << std::endl;
            return;
        }
        std::string filename = evaluator->evaluateStringExpression(parts[1].first);
        bookManager->createFile(filename);
        
    } else if (action == "abrir") {
        if (parts.size() < 2) {
            std::cerr << "Error: libro \"abrir\" necesita el nombre del archivo" << std::endl;
            return;
        }
        std::string filename = evaluator->evaluateStringExpression(parts[1].first);
        bookManager->openFile(filename);
        
    } else if (action == "guardar") {
        if (parts.size() >= 2) {
            std::string filename = evaluator->evaluateStringExpression(parts[1].first);
            bookManager->saveFile(filename);
        } else {
            bookManager->saveFile();
        }
        
    } else if (action == "escribir") {
        if (parts.size() < 3) {
            std::cerr << "Error: libro \"escribir\" necesita clave y valor" << std::endl;
            return;
        }
        std::string key = evaluator->evaluateStringExpression(parts[1].first);
        std::string value = evaluator->evaluateStringExpression(parts[2].first);
        bookManager->writeData(key, value);
        
    } else if (action == "leer") {
        if (parts.size() < 2) {
            std::cerr << "Error: libro \"leer\" necesita la clave" << std::endl;
            return;
        }
        std::string key = evaluator->evaluateStringExpression(parts[1].first);
        std::string result = bookManager->readData(key);
        if (!result.empty()) {
            std::cout << result << std::endl;
        } else {
            std::cout << "Clave '" << key << "' no encontrada" << std::endl;
        }
        
    } else if (action == "obtener") {
        if (parts.size() < 3) {
            std::cerr << "Error: libro \"obtener\" necesita clave y variable destino" << std::endl;
            return;
        }
        std::string key = evaluator->evaluateStringExpression(parts[1].first);
        std::string varName = evaluator->evaluateStringExpression(parts[2].first);
        
        if (!Utils::isValidVariableName(varName)) {
            std::cerr << "Error: '" << varName << "' no es un nombre de variable valido" << std::endl;
            return;
        }
        
        std::string value = bookManager->readData(key);
        if (!value.empty()) {
            if (value.length() >= 2 && value.front() == '[' && value.back() == ']') {
                std::vector<std::string> listItems = Utils::parseArray(value);
                arrayVariables[varName] = listItems;
                
                std::string displayValue = "";
                for (size_t i = 0; i < listItems.size(); i++) {
                    if (i > 0) displayValue += ", ";
                    displayValue += listItems[i];
                }
                stringVariables[varName] = displayValue;
                
            } else if (value.find('.') != std::string::npos) {
                try {
                    double floatVal = std::stod(value);
                    floatVariables[varName] = floatVal;
                } catch (...) {
                    stringVariables[varName] = value;
                }
            } else {
                try {
                    int intVal = std::stoi(value);
                    intVariables[varName] = intVal;
                } catch (...) {
                    stringVariables[varName] = value;
                }
            }
        }
        
    } else if (action == "existe") {
        // Se maneja en executeConditional
        
    } else if (action == "borrar") {
        if (parts.size() < 2) {
            std::cerr << "Error: libro \"borrar\" necesita la clave" << std::endl;
            return;
        }
        std::string key = evaluator->evaluateStringExpression(parts[1].first);
        bookManager->deleteKey(key);
        
    } else if (action == "agregar") {
        if (parts.size() < 3) {
            std::cerr << "Error: libro \"agregar\" necesita clave de lista y valor" << std::endl;
            return;
        }
        std::string key = evaluator->evaluateStringExpression(parts[1].first);
        std::string value = evaluator->evaluateStringExpression(parts[2].first);
        bookManager->addToList(key, value);
        
    } else if (action == "quitar") {
        if (parts.size() < 3) {
            std::cerr << "Error: libro \"quitar\" necesita clave de lista y valor/indice" << std::endl;
            return;
        }
        std::string key = evaluator->evaluateStringExpression(parts[1].first);
        std::string valueOrIndex = evaluator->evaluateStringExpression(parts[2].first);
        
        try {
            int index = std::stoi(valueOrIndex);
            bookManager->removeFromListByIndex(key, index);
        } catch (...) {
            bookManager->removeFromList(key, valueOrIndex);
        }
        
    } else if (action == "listar") {
        bookManager->listAll();
        
    } else if (action == "limpiar") {
        bookManager->clearAll();
        
    } else {
        std::cerr << "Error: Accion de libro no reconocida: " << action << std::endl;
    }
}

void CommandExecutor::executeReceta(const std::string& args) {
    // Comando para importar dependencias
    std::string depName = Utils::trim(args);
    
    // Remover comillas si las tiene
    if (depName.length() >= 2 && depName.front() == '"' && depName.back() == '"') {
        depName = depName.substr(1, depName.length() - 2);
    }
    
    if (dependencyManager->loadDependency(depName)) {
        std::cout << "Dependencia '" << depName << "' cargada exitosamente" << std::endl;
        
        // Si es Discord, inicializar automáticamente si hay config
        if (depName == "la_receta_de_discord") {
            // Intentar cargar configuración desde config.pansos
            if (bookManager->isOpen()) {
                std::string token = bookManager->readData("discord_token");
                std::string botId = bookManager->readData("discord_bot_id");
                
                if (!token.empty() && !botId.empty()) {
                    dependencyManager->initializeDiscord(token, botId);
                }
            }
        }
    } else {
        std::cerr << "Error: No se pudo cargar la dependencia '" << depName << "'" << std::endl;
    }
}

void CommandExecutor::executeDiscord(const std::string& args) {
    if (!dependencyManager->isDependencyLoaded("la_receta_de_discord")) {
        std::cerr << "Error: Primero debe cargar la dependencia Discord con: receta \"la_receta_de_discord\"" << std::endl;
        return;
    }
    
    std::vector<std::pair<std::string, std::string>> parts = Utils::splitExpression(args);
    
    if (parts.empty()) {
        std::cerr << "Error: discord necesita al menos 1 parámetro" << std::endl;
        return;
    }
    
    std::string action = evaluator->evaluateStringExpression(parts[0].first);
    
    if (action == "conectar") {
        if (parts.size() < 3) {
            std::cerr << "Error: discord \"conectar\" necesita token y bot_id" << std::endl;
            return;
        }
        std::string token = evaluator->evaluateStringExpression(parts[1].first);
        std::string botId = evaluator->evaluateStringExpression(parts[2].first);
        
        if (dependencyManager->initializeDiscord(token, botId)) {
            std::cout << "🎯 Conexión establecida con Discord API" << std::endl;
        } else {
            std::cerr << "❌ Error: No se pudo conectar a Discord" << std::endl;
            std::cerr << "💡 Verifica:" << std::endl;
            std::cerr << "   - Token válido del Developer Portal" << std::endl;
            std::cerr << "   - Conexión a internet activa" << std::endl;
            std::cerr << "   - Bot creado en Discord Developer Portal" << std::endl;
        }
        
    } else if (action == "enviar") {
        if (parts.size() < 3) {
            std::cerr << "Error: discord \"enviar\" necesita canal_id y mensaje" << std::endl;
            return;
        }
        std::string channelId = evaluator->evaluateStringExpression(parts[1].first);
        std::string message = evaluator->evaluateStringExpression(parts[2].first);
        
        if (!dependencyManager->sendMessage(channelId, message)) {
            std::cerr << "💡 Posibles causas:" << std::endl;
            std::cerr << "   - Canal ID incorrecto" << std::endl;
            std::cerr << "   - Bot sin permisos en el canal" << std::endl;
            std::cerr << "   - Bot no está en el servidor" << std::endl;
        }
        
    } else if (action == "estado") {
        if (parts.size() < 3) {
            std::cerr << "Error: discord \"estado\" necesita status y actividad" << std::endl;
            return;
        }
        std::string status = evaluator->evaluateStringExpression(parts[1].first);
        std::string activity = evaluator->evaluateStringExpression(parts[2].first);
        dependencyManager->setStatus(status, activity);
        
    } else if (action == "registrar_slash") {
        if (parts.size() < 3) {
            std::cerr << "Error: discord \"registrar_slash\" necesita comando y descripción" << std::endl;
            return;
        }
        std::string command = evaluator->evaluateStringExpression(parts[1].first);
        std::string description = evaluator->evaluateStringExpression(parts[2].first);
        dependencyManager->registerSlashCommand(command, description);
        
    } else if (action == "configurar_prefijo") {
        if (parts.size() < 2) {
            std::cerr << "Error: discord \"configurar_prefijo\" necesita el prefijo" << std::endl;
            return;
        }
        std::string prefix = evaluator->evaluateStringExpression(parts[1].first);
        dependencyManager->setCommandPrefix(prefix);
        
    } else if (action == "registrar_manejador") {
        if (parts.size() < 3) {
            std::cerr << "Error: discord \"registrar_manejador\" necesita tipo y función" << std::endl;
            return;
        }
        std::string type = evaluator->evaluateStringExpression(parts[1].first);
        std::string function = evaluator->evaluateStringExpression(parts[2].first);
        dependencyManager->registerCommandHandler(type, function);
        
    } else if (action == "iniciar_bot_real") {
        if (parts.size() < 2) {
            std::cerr << "Error: discord \"iniciar_bot_real\" necesita canal_id" << std::endl;
            return;
        }
        std::string channelId = evaluator->evaluateStringExpression(parts[1].first);
        dependencyManager->startRealBot(channelId);
        
    } else if (action == "canal") {
        if (parts.size() < 2) {
            std::cerr << "Error: discord \"canal\" necesita canal_id" << std::endl;
            return;
        }
        std::string channelId = evaluator->evaluateStringExpression(parts[1].first);
        std::string channelName = dependencyManager->getChannelInfo(channelId);
        if (!channelName.empty()) {
            std::cout << "📺 Canal: #" << channelName << " (ID: " << channelId << ")" << std::endl;
        } else {
            std::cout << "❌ No se pudo obtener información del canal" << std::endl;
        }
        
    } else if (action == "conectado") {
        if (parts.size() >= 2) {
            std::string varName = evaluator->evaluateStringExpression(parts[1].first);
            if (Utils::isValidVariableName(varName)) {
                intVariables[varName] = dependencyManager->isConnected() ? 1 : 0;
            }
        } else {
            std::cout << (dependencyManager->isConnected() ? "true" : "false") << std::endl;
        }
        
    } else if (action == "mantener_activo") {
        // Comando especial para mantener el bot activo
        std::cout << "🔄 Manteniendo bot activo..." << std::endl;
        std::cout << "💡 Presiona Ctrl+C para detener" << std::endl;
        
        // Loop infinito con verificaciones periódicas
        int counter = 0;
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(30)); // Pausa de 30 segundos
            counter++;
            
            // Verificar conexión cada 2 minutos (4 iteraciones de 30s)
            if (counter % 4 == 0) {
                if (dependencyManager->isConnected()) {
                    std::cout << "🟢 Bot online - " << counter * 30 << " segundos activo" << std::endl;
                } else {
                    std::cout << "🔴 Bot desconectado - Verificando..." << std::endl;
                }
            }
            
            // Mensaje de estado cada 5 minutos (10 iteraciones de 30s)
            if (counter % 10 == 0) {
                std::cout << "⏱️ Bot ha estado activo por " << (counter * 30 / 60) << " minutos" << std::endl;
            }
        }
        
    } else {
        std::cerr << "Error: Acción de discord no reconocida: " << action << std::endl;
        std::cerr << "Acciones disponibles: conectar, enviar, estado, canal, conectado, mantener_activo" << std::endl;
    }
}

void CommandExecutor::executeIngrediente(const std::string& args) {
    size_t equalPos = args.find(" = ");
    if (equalPos == std::string::npos) {
        std::cerr << "Error: Sintaxis incorrecta en declaracion de ingrediente" << std::endl;
        return;
    }
    
    std::string varName = Utils::trim(args.substr(0, equalPos));
    std::string value = Utils::trim(args.substr(equalPos + 3));
    
    if (!Utils::isValidVariableName(varName)) {
        std::cerr << "Error: '" << varName << "' no es un nombre de variable valido" << std::endl;
        return;
    }
    
    if (value.find("pedir_numero(") == 0 || value == "pedir_numero()") {
        std::string result = handleInputFunction(value);
        try {
            intVariables[varName] = std::stoi(result);
        } catch (...) {
            intVariables[varName] = 0;
        }
    } else if (value.find("pedir_nombre(") == 0 || value == "pedir_nombre()") {
        std::string result = handleInputFunction(value);
        stringVariables[varName] = result;
    } else if (Utils::isArrayExpression(value, arrayVariables)) {
        if (value.length() >= 2 && value.front() == '[' && value.back() == ']') {
            arrayVariables[varName] = Utils::parseArray(value);
        } else {
            arrayVariables[varName] = arrayVariables[value];
        }
    } else if (Utils::isStringExpression(value, stringVariables) || 
               (value.length() >= 2 && value.front() == '"' && value.back() == '"')) {
        stringVariables[varName] = evaluator->evaluateStringExpression(value);
    } else if (Utils::isFloatExpression(value, floatVariables)) {
        floatVariables[varName] = evaluator->evaluateFloatExpression(value);
    } else {
        try {
            intVariables[varName] = evaluator->evaluateIntExpression(value);
        } catch (...) {
            stringVariables[varName] = evaluator->evaluateStringExpression(value);
        }
    }
}

void CommandExecutor::executeRecetaDefinition(const std::vector<std::string>& lines, int& currentLine) {
    std::string line = Utils::trim(lines[currentLine]);
    
    size_t nameStart = line.find("receta ") + 7;
    size_t parenPos = line.find('(');
    size_t parenEnd = line.find(')');
    
    if (parenPos == std::string::npos || parenEnd == std::string::npos) {
        std::cerr << "Error: Sintaxis incorrecta en receta" << std::endl;
        return;
    }
    
    std::string funcName = Utils::trim(line.substr(nameStart, parenPos - nameStart));
    std::string paramStr = Utils::trim(line.substr(parenPos + 1, parenEnd - parenPos - 1));
    
    if (!Utils::isValidVariableName(funcName)) {
        std::cerr << "Error: Nombre de receta invalido: " << funcName << std::endl;
        return;
    }
    
    Function func;
    func.parameters = Utils::parseParameters(paramStr);
    func.startLine = currentLine;
    
    currentLine++;
    while (currentLine < static_cast<int>(lines.size())) {
        std::string bodyLine = Utils::trim(lines[currentLine]);
        if (bodyLine == "listo") break;
        func.body.push_back(lines[currentLine]);
        currentLine++;
    }
    
    functions[funcName] = func;
}

void CommandExecutor::executeCocinar(const std::string& args) {
    size_t parenPos = args.find('(');
    size_t parenEnd = args.find(')');
    
    if (parenPos == std::string::npos || parenEnd == std::string::npos) {
        std::cerr << "Error: Sintaxis incorrecta en cocinar" << std::endl;
        return;
    }
    
    std::string funcName = Utils::trim(args.substr(0, parenPos));
    std::string argsStr = Utils::trim(args.substr(parenPos + 1, parenEnd - parenPos - 1));
    
    if (functions.find(funcName) == functions.end()) {
        std::cerr << "Error: Receta no encontrada: " << funcName << std::endl;
        return;
    }
    
    Function& func = functions[funcName];
    std::vector<std::string> callArgs = Utils::parseParameters(argsStr);
    
    auto savedIntVars = intVariables;
    auto savedStringVars = stringVariables;
    auto savedFloatVars = floatVariables;
    auto savedArrayVars = arrayVariables;
    
    for (size_t i = 0; i < func.parameters.size() && i < callArgs.size(); i++) {
        std::string paramName = func.parameters[i];
        std::string argValue = callArgs[i];
        
        if (Utils::isStringExpression(argValue, stringVariables) || 
            (argValue.length() >= 2 && argValue.front() == '"' && argValue.back() == '"')) {
            stringVariables[paramName] = evaluator->evaluateStringExpression(argValue);
        } else if (Utils::isFloatExpression(argValue, floatVariables)) {
            floatVariables[paramName] = evaluator->evaluateFloatExpression(argValue);
        } else {
            intVariables[paramName] = evaluator->evaluateIntExpression(argValue);
        }
    }
    
    for (int i = 0; i < static_cast<int>(func.body.size()); i++) {
        executeLine(func.body, i);
    }
    
    intVariables = savedIntVars;
    stringVariables = savedStringVars;
    floatVariables = savedFloatVars;
    arrayVariables = savedArrayVars;
}

void CommandExecutor::executeConditional(const std::vector<std::string>& lines, int& currentLine) {
    std::string line = Utils::trim(lines[currentLine]);
    int listoLine = findMatchingListo(lines, currentLine);
    
    if (listoLine == -1) {
        std::cerr << "Error: No se encontro 'listo' para cerrar el condicional" << std::endl;
        return;
    }
    
    bool conditionMet = false;
    
    if (line.find("probar ") == 0 && line.back() == ':') {
        std::string condition = line.substr(7);
        condition = condition.substr(0, condition.length() - 1);
        
        if (condition.find("libro \"existe\"") != std::string::npos) {
            size_t start = condition.find("\"existe\", \"") + 11;
            size_t end = condition.find("\"", start);
            if (end != std::string::npos) {
                std::string key = condition.substr(start, end - start);
                conditionMet = bookManager->existsKey(key);
            }
        } else {
            conditionMet = evaluator->evaluateCondition(condition);
        }
        
        if (conditionMet) {
            currentLine++;
            while (currentLine < listoLine) {
                std::string innerLine = Utils::trim(lines[currentLine]);
                if (innerLine.find("sino ") == 0 || innerLine == "alternativa:") {
                    break;
                }
                executeLine(lines, currentLine);
                currentLine++;
            }
        }
    }
    
    if (!conditionMet) {
        currentLine++;
        while (currentLine < listoLine) {
            std::string innerLine = Utils::trim(lines[currentLine]);
            
            if (innerLine.find("sino ") == 0 && innerLine.back() == ':') {
                std::string condition = innerLine.substr(5);
                condition = condition.substr(0, condition.length() - 1);
                
                if (evaluator->evaluateCondition(condition)) {
                    conditionMet = true;
                    currentLine++;
                    while (currentLine < listoLine) {
                        std::string nextLine = Utils::trim(lines[currentLine]);
                        if (nextLine.find("sino ") == 0 || nextLine == "alternativa:") {
                            break;
                        }
                        executeLine(lines, currentLine);
                        currentLine++;
                    }
                    break;
                }
            } else if (innerLine == "alternativa:") {
                if (!conditionMet) {
                    currentLine++;
                    while (currentLine < listoLine) {
                        executeLine(lines, currentLine);
                        currentLine++;
                    }
                }
                break;
            }
            currentLine++;
        }
        std::cerr << "Acciones disponibles: conectar, enviar, estado, canal, conectado, registrar_slash, configurar_prefijo, registrar_manejador, iniciar_bot_real, mantener_activo" << std::endl;
    }
    
    currentLine = listoLine;
}

void CommandExecutor::executeLine(const std::vector<std::string>& lines, int& currentLine) {
    std::string originalLine = lines[currentLine];
    std::string line = Utils::trim(Utils::removeComments(originalLine));
    
    if (line.empty()) return;

    try {
        if (line.find("hornear ") == 0) {
            executeHornear(line.substr(8));
        } else if (line.find("ingrediente ") == 0) {
            executeIngrediente(line.substr(12));
        } else if (line.find("probar ") == 0) {
            executeConditional(lines, currentLine);
        } else if (line.find("receta ") == 0 && line.find("(") != std::string::npos) {
            executeRecetaDefinition(lines, currentLine);
        } else if (line.find("cocinar ") == 0) {
            executeCocinar(line.substr(8));
        } else if (line.find("mostrador ") == 0) {
            executeMostrador(line.substr(10));
        } else if (line.find("escribir ") == 0) {
            executeEscribir(line.substr(9));
        } else if (line.find("libro ") == 0) {
            executeLibro(line.substr(6));
        } else if (line.find("receta ") == 0 && line.find("(") == std::string::npos) {
            executeReceta(line.substr(7));
        } else if (line.find("discord ") == 0) {
            executeDiscord(line.substr(8));
        } else if (line.find("juego ") == 0) {
            executeJuego(line.substr(6));
        } else if (line.find("objeto ") == 0) {
            executeObjeto(line.substr(7));
        } else if (line.find("mover ") == 0) {
            executeMover(line.substr(6));
        } else if (line.find("posicionar ") == 0) {
            executePosicionar(line.substr(11));
        } else if (line.find("ocultar ") == 0) {
            executeOcultar(line.substr(8));
        } else if (line != "listo" && line != "alternativa:" && line.find("sino ") != 0) {
            std::cerr << "Error: Linea no reconocida: " << line << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error en la linea " << (currentLine + 1) << ": " << e.what() << std::endl;
    }
}