#pragma once
#include <string>
#include <unordered_map>

class VersionMapping {
public:
    static void initialize(const std::string& version);
    static std::string getClassName(const std::string& key);
    static std::string getMethodName(const std::string& key);
    static std::string getFieldName(const std::string& key);

private:
    static std::unordered_map<std::string, std::string> classMap;
    static std::unordered_map<std::string, std::string> methodMap;
    static std::unordered_map<std::string, std::string> fieldMap;
};