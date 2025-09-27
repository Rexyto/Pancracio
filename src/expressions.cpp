#include "expressions.h"
#include "utils.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <cmath>
#include <sstream>

ExpressionEvaluator::ExpressionEvaluator(
    std::map<std::string, int>& intVars,
    std::map<std::string, std::string>& stringVars,
    std::map<std::string, double>& floatVars,
    std::map<std::string, std::vector<std::string>>& arrayVars,
    std::vector<WindowInfo>& wins
) : intVariables(intVars), stringVariables(stringVars), floatVariables(floatVars), 
    arrayVariables(arrayVars), windows(wins) {}

std::string ExpressionEvaluator::evaluateStringExpression(const std::string& expr) {
    std::string trimmed = Utils::trim(expr);
    
    // String simple con comillas sin concatenaciÃ³n
    if (trimmed.length() >= 2 && trimmed.front() == '"' && trimmed.back() == '"') {
        if (trimmed.find(" + ") == std::string::npos && trimmed.find(", ") == std::string::npos) {
            return trimmed.substr(1, trimmed.length() - 2);
        }
    }
    
    // ARREGLO CRÃTICO: DetecciÃ³n mejorada de concatenaciÃ³n
    std::vector<std::string> parts;
    std::string current = "";
    bool inQuotes = false;
    int parenDepth = 0;
    
    for (size_t i = 0; i < trimmed.length(); i++) {
        char c = trimmed[i];
        
        // Manejar comillas
        if (c == '"' && (i == 0 || trimmed[i-1] != '\\')) {
            inQuotes = !inQuotes;
            current += c;
            continue;
        }
        
        // Manejar parÃ©ntesis solo fuera de comillas
        if (!inQuotes) {
            if (c == '(') parenDepth++;
            else if (c == ')') parenDepth--;
        }
        
        // Solo procesar operadores fuera de comillas y parÃ©ntesis
        if (!inQuotes && parenDepth == 0) {
            // Buscar " + " para concatenaciÃ³n
            if (i >= 1 && i + 1 < trimmed.length() && 
                trimmed.substr(i-1, 3) == " + ") {
                // Retroceder para no incluir el espacio antes del +
                if (!current.empty() && current.back() == ' ') {
                    current.pop_back();
                }
                parts.push_back(Utils::trim(current));
                current = "";
                i++; // Saltar el espacio despuÃ©s del +
                continue;
            }
            // Buscar ", " para separaciÃ³n
            else if (c == ',' && i + 1 < trimmed.length() && trimmed[i+1] == ' ') {
                parts.push_back(Utils::trim(current));
                current = "";
                i++; // Saltar el espacio despuÃ©s de la coma
                continue;
            }
        }
        
        current += c;
    }
    
    parts.push_back(Utils::trim(current));
    
    if (parts.size() == 1) {
        return evaluateSinglePart(parts[0]);
    }
    
    // CRÃTICO: Concatenar todas las partes
    std::string result = "";
    for (size_t i = 0; i < parts.size(); i++) {
        std::string partValue = evaluateSinglePart(parts[i]);
        result += partValue;
    }
    
    return result;
}

std::string ExpressionEvaluator::evaluateSinglePart(const std::string& part) {
    std::string trimmed = Utils::trim(part);
    
    // String literal
    if (trimmed.length() >= 2 && trimmed.front() == '"' && trimmed.back() == '"') {
        return trimmed.substr(1, trimmed.length() - 2);
    }
    
    // CRÃTICO: Primero buscar en variables string
    auto stringIt = stringVariables.find(trimmed);
    if (stringIt != stringVariables.end()) {
        return stringIt->second;
    }
    
    // CRÃTICO: Buscar en variables array y formatear correctamente
    auto arrayIt = arrayVariables.find(trimmed);
    if (arrayIt != arrayVariables.end()) {
        const std::vector<std::string>& arr = arrayIt->second;
        if (arr.empty()) return "";
        
        std::string result = "";
        for (size_t i = 0; i < arr.size(); i++) {
            if (i > 0) result += ", ";
            result += arr[i]; // SIN COMILLAS EXTRA
        }
        return result;
    }
    
    // Variables enteras
    auto intIt = intVariables.find(trimmed);
    if (intIt != intVariables.end()) {
        return std::to_string(intIt->second);
    }
    
    // Variables float
    auto floatIt = floatVariables.find(trimmed);
    if (floatIt != floatVariables.end()) {
        double val = floatIt->second;
        if (val == (int)val) {
            return std::to_string((int)val);
        } else {
            std::ostringstream oss;
            oss.precision(2);
            oss << std::fixed << val;
            std::string str = oss.str();
            while (str.length() > 1 && str.back() == '0' && str[str.length()-2] != '.') {
                str.pop_back();
            }
            return str;
        }
    }
    
    // NÃºmero directo
    try {
        int intVal = std::stoi(trimmed);
        return std::to_string(intVal);
    } catch (...) {}
    
    try {
        double doubleVal = std::stod(trimmed);
        if (doubleVal == (int)doubleVal) {
            return std::to_string((int)doubleVal);
        } else {
            std::ostringstream oss;
            oss.precision(2);
            oss << std::fixed << doubleVal;
            std::string str = oss.str();
            while (str.length() > 1 && str.back() == '0' && str[str.length()-2] != '.') {
                str.pop_back();
            }
            return str;
        }
    } catch (...) {}
    
    return trimmed;
}

double ExpressionEvaluator::evaluateFloatExpression(const std::string& expr) {
    std::string trimmed = Utils::trim(expr);
    
    auto floatIt = floatVariables.find(trimmed);
    if (floatIt != floatVariables.end()) {
        return floatIt->second;
    }
    
    auto intIt = intVariables.find(trimmed);
    if (intIt != intVariables.end()) {
        return static_cast<double>(intIt->second);
    }
    
    try {
        return std::stod(trimmed);
    } catch (...) {}
    
    std::vector<std::string> ops = {"+", "-", "*", "/"};
    for (const std::string& op : ops) {
        std::string opPattern = " " + op + " ";
        size_t pos = trimmed.find(opPattern);
        if (pos != std::string::npos) {
            std::string left = Utils::trim(trimmed.substr(0, pos));
            std::string right = Utils::trim(trimmed.substr(pos + opPattern.length()));
            
            double leftVal = evaluateFloatExpression(left);
            double rightVal = evaluateFloatExpression(right);
            
            if (op == "+") return leftVal + rightVal;
            if (op == "-") return leftVal - rightVal;
            if (op == "*") return leftVal * rightVal;
            if (op == "/") {
                if (rightVal != 0.0) {
                    return leftVal / rightVal;
                } else {
                    std::cerr << "Error: DivisiÃ³n por cero" << std::endl;
                    return 0.0;
                }
            }
        }
    }
    
    return 0.0;
}

int ExpressionEvaluator::evaluateIntExpression(const std::string& expr) {
    std::string trimmed = Utils::trim(expr);
    
    if (trimmed.find("aleatorio(") == 0) {
        size_t start = trimmed.find('(') + 1;
        size_t end = trimmed.find_last_of(')');
        if (start < end) {
            std::string params = trimmed.substr(start, end - start);
            size_t commaPos = params.find(',');
            if (commaPos != std::string::npos) {
                int min = evaluateIntExpression(Utils::trim(params.substr(0, commaPos)));
                int max = evaluateIntExpression(Utils::trim(params.substr(commaPos + 1)));
                
                static std::random_device rd;
                static std::mt19937 gen(rd());
                std::uniform_int_distribution<> dis(min, max);
                return dis(gen);
            }
        }
        return 0;
    }
    
    auto intIt = intVariables.find(trimmed);
    if (intIt != intVariables.end()) {
        return intIt->second;
    }
    
    auto floatIt = floatVariables.find(trimmed);
    if (floatIt != floatVariables.end()) {
        return static_cast<int>(floatIt->second);
    }
    
    try {
        return std::stoi(trimmed);
    } catch (...) {}
    
    std::vector<std::string> ops = {"+", "-", "*", "/", "%"};
    for (const std::string& op : ops) {
        std::string opPattern = " " + op + " ";
        size_t pos = trimmed.find(opPattern);
        if (pos != std::string::npos) {
            std::string left = Utils::trim(trimmed.substr(0, pos));
            std::string right = Utils::trim(trimmed.substr(pos + opPattern.length()));
            
            int leftVal = evaluateIntExpression(left);
            int rightVal = evaluateIntExpression(right);
            
            if (op == "+") return leftVal + rightVal;
            if (op == "-") return leftVal - rightVal;
            if (op == "*") return leftVal * rightVal;
            if (op == "/") {
                if (rightVal != 0) {
                    return leftVal / rightVal;
                } else {
                    std::cerr << "Error: DivisiÃ³n por cero" << std::endl;
                    return 0;
                }
            }
            if (op == "%") {
                if (rightVal != 0) {
                    return leftVal % rightVal;
                } else {
                    std::cerr << "Error: MÃ³dulo por cero" << std::endl;
                    return 0;
                }
            }
        }
    }
    
    return 0;
}

bool ExpressionEvaluator::evaluateCondition(const std::string& condition) {
    std::string trimmed = Utils::trim(condition);
    
    if (trimmed.find("tecla(") == 0) {
        size_t start = trimmed.find('(') + 1;
        size_t end = trimmed.find_last_of(')');
        if (start < end) {
            std::string keyName = Utils::trim(trimmed.substr(start, end - start));
            keyName = evaluateStringExpression(keyName);
            
            int vkCode = Utils::getVirtualKeyCode(keyName);
            if (vkCode != -1) {
                for (const auto& win : windows) {
                    if (win.isGameWindow && win.keysPressed[vkCode]) {
                        return true;
                    }
                }
            }
        }
        return false;
    }
    
    std::vector<std::string> compOps = {"==", "!=", "<=", ">=", "<", ">"};
    for (const std::string& op : compOps) {
        size_t pos = trimmed.find(" " + op + " ");
        if (pos != std::string::npos) {
            std::string left = Utils::trim(trimmed.substr(0, pos));
            std::string right = Utils::trim(trimmed.substr(pos + op.length() + 2));
            
            double leftVal = evaluateFloatExpression(left);
            double rightVal = evaluateFloatExpression(right);
            
            if (op == "==") return std::abs(leftVal - rightVal) < 0.0001;
            if (op == "!=") return std::abs(leftVal - rightVal) >= 0.0001;
            if (op == "<=") return leftVal <= rightVal;
            if (op == ">=") return leftVal >= rightVal;
            if (op == "<") return leftVal < rightVal;
            if (op == ">") return leftVal > rightVal;
        }
    }
    
    return false;
}