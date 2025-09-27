#pragma once
#include <string>
#include <map>
#include <vector>
#include "structures.h"

class ExpressionEvaluator {
private:
    std::map<std::string, int>& intVariables;
    std::map<std::string, std::string>& stringVariables;
    std::map<std::string, double>& floatVariables;
    std::map<std::string, std::vector<std::string>>& arrayVariables;
    std::vector<WindowInfo>& windows;

    std::string evaluateSinglePart(const std::string& part);

public:
    ExpressionEvaluator(
        std::map<std::string, int>& intVars,
        std::map<std::string, std::string>& stringVars,
        std::map<std::string, double>& floatVars,
        std::map<std::string, std::vector<std::string>>& arrayVars,
        std::vector<WindowInfo>& windows
    );

    std::string evaluateStringExpression(const std::string& expr);
    double evaluateFloatExpression(const std::string& expr);
    int evaluateIntExpression(const std::string& expr);
    bool evaluateCondition(const std::string& condition);
};
