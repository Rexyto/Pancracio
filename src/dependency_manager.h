#pragma once
#include <string>
#include <map>
#include <vector>
#include <memory>

class DependencyManager {
private:
    std::map<std::string, std::string> loadedDependencies;
    std::string dependenciesPath;
    
    bool loadDependencyInfo(const std::string& depName, std::map<std::string, std::string>& info);
    bool validateDependency(const std::string& depName);
    std::string findDependencyPath(const std::string& depName);
    std::string makeHttpRequest(const std::string& method, const std::string& endpoint, const std::string& data = "");

public:
    DependencyManager();
    ~DependencyManager();
    
    bool loadDependency(const std::string& depName);
    bool isDependencyLoaded(const std::string& depName);
    std::string getDependencyPath(const std::string& depName);
    std::vector<std::string> getAvailableDependencies();
    
    // Métodos específicos para Discord (implementación real)
    bool initializeDiscord(const std::string& token, const std::string& botId);
    bool sendMessage(const std::string& channelId, const std::string& message);
    bool setStatus(const std::string& status, const std::string& activity);
    std::string getChannelInfo(const std::string& channelId);
    bool isConnected();
    void cleanupDiscord();
    
    // Nuevos métodos para bot real
    bool registerSlashCommand(const std::string& command, const std::string& description);
    void setCommandPrefix(const std::string& prefix);
    void registerCommandHandler(const std::string& type, const std::string& function);
    void startRealBot(const std::string& channelId);
};