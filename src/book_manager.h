#pragma once
#include <string>
#include <map>
#include <vector>
#include <fstream>

class BookManager {
private:
    std::string currentFile;
    std::map<std::string, std::string> data;
    bool isFileOpen;
    
    std::string parseValue(const std::string& line, const std::string& type);
    std::string formatValue(const std::string& value, const std::string& type);
    std::string detectType(const std::string& value);
    std::vector<std::string> parseList(const std::string& listStr);
    std::string formatList(const std::vector<std::string>& list);
    bool isValidKey(const std::string& key);

public:
    BookManager();
    ~BookManager();
    
    // Operaciones de archivo
    bool createFile(const std::string& filename);
    bool openFile(const std::string& filename);
    bool saveFile(const std::string& filename = "");
    
    // Operaciones de datos
    void writeData(const std::string& key, const std::string& value);
    std::string readData(const std::string& key);
    bool existsKey(const std::string& key);
    void deleteKey(const std::string& key);
    
    // Operaciones de lista
    void addToList(const std::string& key, const std::string& value);
    void removeFromList(const std::string& key, const std::string& value);
    void removeFromListByIndex(const std::string& key, int index);
    
    // Utilidades
    void listAll();
    void clearAll();
    void cleanCorruptedData();
    std::string getCurrentFile() const { return currentFile; }
    bool isOpen() const { return isFileOpen; }
};