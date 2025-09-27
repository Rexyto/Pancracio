#include "utils.h"
#include <sstream>
#include <algorithm>
#include <cctype>

std::vector<std::string> Utils::keywords = {
    "hornear", "ingrediente", "probar", "alternativa", "sino", "listo", 
    "pedir_numero", "pedir_nombre", "receta", "cocinar", "mostrador",
    "entero", "texto", "decimal", "lista", "escribir", "boton", "entrada", "libro",
    "discord",
    "juego", "objeto", "mover", "dibujar", "colision", "tecla", "temporizador", 
    "aleatorio", "posicionar", "objeto_x", "objeto_y", "ocultar"
};

std::string Utils::trim(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

bool Utils::isKeyword(const std::string& word) {
    return std::find(keywords.begin(), keywords.end(), word) != keywords.end();
}

bool Utils::isValidVariableName(const std::string& name) {
    if (name.empty() || isKeyword(name)) return false;
    if (!std::isalpha(name[0]) && name[0] != '_') return false;
    for (char c : name) {
        if (!std::isalnum(c) && c != '_') return false;
    }
    return true;
}

bool Utils::isStringExpression(const std::string& expr, const std::map<std::string, std::string>& stringVars) {
    std::string trimmed = trim(expr);
    if (trimmed.length() >= 2 && trimmed.front() == '"' && trimmed.back() == '"') return true;
    if (stringVars.find(trimmed) != stringVars.end()) return true;
    return false;
}

bool Utils::isFloatExpression(const std::string& value, const std::map<std::string, double>& floatVars) {
    std::string trimmedValue = trim(value);
    return trimmedValue.find('.') != std::string::npos || floatVars.find(trimmedValue) != floatVars.end();
}

bool Utils::isArrayExpression(const std::string& value, const std::map<std::string, std::vector<std::string>>& arrayVars) {
    std::string trimmedValue = trim(value);
    return (trimmedValue.length() >= 2 && trimmedValue.front() == '[' && trimmedValue.back() == ']') || 
           arrayVars.find(trimmedValue) != arrayVars.end();
}

std::string Utils::removeComments(const std::string& line) {
    size_t commentPos = line.find('#');
    if (commentPos != std::string::npos) {
        bool inQuotes = false;
        for (size_t i = 0; i < commentPos; i++) {
            if (line[i] == '"') {
                inQuotes = !inQuotes;
            }
        }
        if (!inQuotes) {
            return trim(line.substr(0, commentPos));
        }
    }
    return line;
}

std::vector<std::string> Utils::parseParameters(const std::string& paramStr) {
    std::vector<std::string> params;
    if (paramStr.empty()) return params;
    
    std::stringstream ss(paramStr);
    std::string param;
    while (std::getline(ss, param, ',')) {
        params.push_back(trim(param));
    }
    return params;
}

std::vector<std::string> Utils::parseArray(const std::string& arrayStr) {
    std::vector<std::string> elements;
    if (arrayStr.length() < 2 || arrayStr.front() != '[' || arrayStr.back() != ']') {
        return elements;
    }
    
    std::string content = arrayStr.substr(1, arrayStr.length() - 2);
    return parseParameters(content);
}

std::vector<std::pair<std::string, std::string>> Utils::splitExpression(const std::string& expr) {
    std::vector<std::pair<std::string, std::string>> parts;
    std::string current = "";
    bool inQuotes = false;
    int parenDepth = 0;
    
    for (size_t i = 0; i < expr.length(); i++) {
        char c = expr[i];
        
        // Manejar comillas
        if (c == '"' && (i == 0 || expr[i-1] != '\\')) {
            inQuotes = !inQuotes;
            current += c;
            continue;
        }
        
        // Manejar paréntesis solo fuera de comillas
        if (!inQuotes) {
            if (c == '(') parenDepth++;
            else if (c == ')') parenDepth--;
        }
        
        // CRÍTICO: Solo procesar comas como separadores, NO el operador +
        if (!inQuotes && parenDepth == 0) {
            // Solo buscar ", " para separación de parámetros
            if (c == ',' && i + 1 < expr.length() && expr[i+1] == ' ') {
                parts.push_back({trim(current), ","});
                current = "";
                i++; // Saltar el espacio después de la coma
                continue;
            }
            // REMOVIDO: No procesar '+' como separador aquí
            // El '+' debe ser manejado por evaluateStringExpression, no por splitExpression
        }
        
        current += c;
    }
    
    if (!current.empty()) {
        parts.push_back({trim(current), ""});
    }
    
    return parts;
}

COLORREF Utils::getColorFromName(const std::string& colorName) {
    std::string color = colorName;
    std::transform(color.begin(), color.end(), color.begin(), ::tolower);
    
    if (color == "rojo") return RGB(255, 100, 100);
    if (color == "verde") return RGB(100, 255, 100);
    if (color == "azul") return RGB(100, 150, 255);
    if (color == "amarillo") return RGB(255, 255, 150);
    if (color == "negro") return RGB(50, 50, 50);
    if (color == "blanco") return RGB(245, 245, 245);
    if (color == "gris") return RGB(180, 180, 180);
    if (color == "naranja") return RGB(255, 200, 100);
    if (color == "rosa") return RGB(255, 192, 203);
    if (color == "morado") return RGB(200, 150, 255);
    
    return RGB(245, 245, 245);
}

int Utils::getVirtualKeyCode(const std::string& keyName) {
    std::string key = keyName;
    std::transform(key.begin(), key.end(), key.begin(), ::tolower);
    
    if (key == "arriba") return VK_UP;
    if (key == "abajo") return VK_DOWN;
    if (key == "izquierda") return VK_LEFT;
    if (key == "derecha") return VK_RIGHT;
    if (key == "espacio") return VK_SPACE;
    if (key == "enter") return VK_RETURN;
    if (key == "escape") return VK_ESCAPE;
    if (key == "a") return 'A';
    if (key == "s") return 'S';
    if (key == "d") return 'D';
    if (key == "w") return 'W';
    
    return -1;
}