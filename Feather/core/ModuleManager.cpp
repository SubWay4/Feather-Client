#include "pch.h"
#include "ModuleManager.h"
#include "../hooks/LunarClientHook.h"
#include <iostream>

void ModuleManager::toggleModule(const std::string& moduleName, bool enabled) {
    modules[moduleName] = enabled;

    if (moduleName == "AimAssist") {
        LunarClientHook::toggleAimAssist(enabled);
    }
    else if (moduleName == "Reach") {
        LunarClientHook::toggleReach(enabled);
    }
    else if (moduleName == "FullBright") {
        LunarClientHook::toggleFullBright(enabled);
    }
    else if (moduleName == "Velocity") {
        LunarClientHook::toggleVelocity(enabled);
    }
    else if (moduleName == "LeftClicker") {
        LunarClientHook::toggleLeftClicker(enabled);
    }
    else if (moduleName == "FastPlace") {
        LunarClientHook::toggleFastPlace(enabled);
    }
    else if (moduleName == "LegitScaffold") {
        LunarClientHook::toggleLegitScaffold(enabled);
    }
    else if (moduleName == "Sprint") {
        LunarClientHook::toggleSprint(enabled);
    }
    else if (moduleName == "NoFall") {
        LunarClientHook::toggleNoFall(enabled);
    }
    else {
        std::cout << "Unknown module: " << moduleName << std::endl;
    }
}

bool ModuleManager::isModuleEnabled(const std::string& moduleName) const {
    auto it = modules.find(moduleName);
    return (it != modules.end()) ? it->second : false;
}
