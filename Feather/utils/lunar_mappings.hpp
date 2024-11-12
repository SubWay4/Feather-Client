
#pragma once
#include <string>
#include <unordered_map>
#include <vector>

namespace lunar_mappings {

    struct FieldInfo {
        std::string name;
        std::string obfuscated;
        std::string signature;
        bool isStatic;
    };

    struct MethodInfo {
        std::string name;
        std::string obfuscated;
        std::string signature;
        bool isStatic;
    };

    struct ClassInfo {
        std::string obfuscated;
        std::vector<FieldInfo> fields;
        std::vector<MethodInfo> methods;
    };

    extern const std::unordered_map<std::string, ClassInfo> mappings;

}
