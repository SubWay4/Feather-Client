#include "pch.h"
#include "VersionMapping.h"

std::unordered_map<std::string, std::string> VersionMapping::classMap;
std::unordered_map<std::string, std::string> VersionMapping::methodMap;
std::unordered_map<std::string, std::string> VersionMapping::fieldMap;

void VersionMapping::initialize(const std::string& version) {
    if (version == "1.8.9") {
        classMap["Minecraft"] = "net/minecraft/client/Minecraft";
        methodMap["getPlayer"] = "thePlayer";
        fieldMap["theMinecraft"] = "theMinecraft";
    }
    else if (version == "1.12.2") {
        classMap["Minecraft"] = "net/minecraft/client/Minecraft";
        methodMap["getPlayer"] = "player";
        fieldMap["theMinecraft"] = "INSTANCE";
    }
    // Agregar más mapeos específicos de versión según sea necesario
}

std::string VersionMapping::getClassName(const std::string& key) {
    return classMap[key];
}

std::string VersionMapping::getMethodName(const std::string& key) {
    return methodMap[key];
}

std::string VersionMapping::getFieldName(const std::string& key) {
    return fieldMap[key];
}