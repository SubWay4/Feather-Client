#pragma once
#include <unordered_map>
#include <string>

class ModuleManager {
public:
    static ModuleManager& getInstance() {
        static ModuleManager instance;
        return instance;
    }

    void toggleModule(const std::string& moduleName, bool enabled);
    bool isModuleEnabled(const std::string& moduleName) const;

private:
    ModuleManager() {} // Private constructor to ensure singleton
    std::unordered_map<std::string, bool> modules;
};
