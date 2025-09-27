#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <map>
#ifdef _WIN32
#include <windows.h>
#endif

class Utils {
public:
    static std::string trim(const std::string& str);
    static bool isKeyword(const std::string& word);
    static bool isValidVariableName(const std::string& name);
    static bool isStringExpression(const std::string& expr, const std::map<std::string, std::string>& stringVars);
    static bool isFloatExpression(const std::string& value, const std::map<std::string, double>& floatVars);
    static bool isArrayExpression(const std::string& value, const std::map<std::string, std::vector<std::string>>& arrayVars);
    static std::string removeComments(const std::string& line);
    static std::vector<std::string> parseParameters(const std::string& paramStr);
    static std::vector<std::string> parseArray(const std::string& arrayStr);
    static std::vector<std::pair<std::string, std::string>> splitExpression(const std::string& expr);
    static COLORREF getColorFromName(const std::string& colorName);
    static int getVirtualKeyCode(const std::string& keyName);
    
private:
    static std::vector<std::string> keywords;
};