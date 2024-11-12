#pragma once

#include <string>
#include <unordered_map>

class ModuleManager {
public:
    void toggleModule(const std::string& moduleName, bool enabled);
    bool isModuleEnabled(const std::string& moduleName) const;

private:
    std::unordered_map<std::string, bool> modules = {
    {"AimAssist", false},
    {"Reach", false},
    {"FullBright", false},
    {"Velocity", false},
    {"LeftClicker", false},
    {"FastPlace", false},
    {"LegitScaffold", false},
    {"Sprint", false},
    {"NoFall", false}
    };
};