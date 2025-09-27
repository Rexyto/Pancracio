#pragma once
#include <string>
#include <map>
#include <vector>
#include "structures.h"
#include "expressions.h"
#include "window_manager.h"
#include "book_manager.h"
#include "dependency_manager.h"

class CommandExecutor {
private:
    std::map<std::string, int> intVariables;
    std::map<std::string, std::string> stringVariables;
    std::map<std::string, double> floatVariables;
    std::map<std::string, std::vector<std::string>> arrayVariables;
    std::map<std::string, Function> functions;
    
    ExpressionEvaluator* evaluator;
    WindowManager* windowManager;
    BookManager* bookManager;
    DependencyManager* dependencyManager;

    std::string handleInputFunction(const std::string& funcCall);
    int findMatchingListo(const std::vector<std::string>& lines, int startLine);
    
public:
    CommandExecutor();
    ~CommandExecutor();

    void executeHornear(const std::string& args);
    void executeMostrador(const std::string& args);
    void executeJuego(const std::string& args);
    void executeObjeto(const std::string& args);
    void executeMover(const std::string& args);
    void executeColision(const std::string& args, bool& result);
    void executeOcultar(const std::string& args);
    void executePosicionar(const std::string& args);
    void executeEscribir(const std::string& args);
    void executeLibro(const std::string& args);
    void executeReceta(const std::string& args);
    void executeDiscord(const std::string& args);
    void executeIngrediente(const std::string& args);
    void executeRecetaDefinition(const std::vector<std::string>& lines, int& currentLine);
    void executeCocinar(const std::string& args);
    void executeConditional(const std::vector<std::string>& lines, int& currentLine);
    void executeLine(const std::vector<std::string>& lines, int& currentLine);
    
    void runMessageLoop() { windowManager->runMessageLoop(); }
    bool hasWindows() { return !windowManager->getWindows().empty(); }
};