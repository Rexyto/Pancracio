#pragma once
#include <string>
#include <vector>
#include "command_executor.h"

class PancracioInterpreter {
private:
    CommandExecutor* executor;

public:
    PancracioInterpreter();
    ~PancracioInterpreter();
    
    void executeFile(const std::string& filename);
    void showVersion();
};