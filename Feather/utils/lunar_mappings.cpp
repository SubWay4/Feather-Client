
#include "lunar_mappings.hpp"
#include <json.hpp>

namespace lunar_mappings {

    const char mappings_lunar_1_8_9[] = R"(
{
"java/lang/Object": {
"obfuscated": "java/lang/Object",
"fields": [],
"methods": [
{
"name": "equals",
"obfuscated": "equals",
"signature": "(Ljava/lang/Object;)Z",
"static": false
},
{
"name": "toString",
"obfuscated": "toString",
"signature": "()Ljava/lang/String;",
"static": false
}
]
},
// ... (aquí va el resto del contenido JSON que proporcionaste)
"net/minecraft/network/play/server/S14PacketEntity": {
"obfuscated": "net/minecraft/network/play/server/S14PacketEntity",
"fields": [
{
"name": "entityId",
"obfuscated": "entityId",
"signature": "I",
"static": false
},
{
"name": "posX",
"obfuscated": "posX",
"signature": "B",
"static": false
},
{
"name": "posY",
"obfuscated": "posY",
"signature": "B",
"static": false
},
{
"name": "posZ",
"obfuscated": "posZ",
"signature": "B",
"static": false
},
{
"name": "yaw",
"obfuscated": "yaw",
"signature": "B",
"static": false
},
{
"name": "pitch",
"obfuscated": "pitch",
"signature": "B",
"static": false
},
{
"name": "onGround",
"obfuscated": "onGround",
"signature": "Z",
"static": false
},
{
"name": "hasRot",
"obfuscated": "field_149069_g",
"signature": "Z",
"static": false
}
],
"methods": []
}
})";

    const std::unordered_map<std::string, ClassInfo> mappings = []() {
        std::unordered_map<std::string, ClassInfo> result;
        nlohmann::json j = nlohmann::json::parse(mappings_lunar_1_8_9);
        for (auto& [class_name, class_data] : j.items()) {
            ClassInfo ci;
            ci.obfuscated = class_data["obfuscated"];

            for (auto& field : class_data["fields"]) {
                FieldInfo fi;
                fi.name = field["name"];
                fi.obfuscated = field["obfuscated"];
                fi.signature = field["signature"];
                fi.isStatic = field["static"];
                ci.fields.push_back(fi);
            }

            for (auto& method : class_data["methods"]) {
                MethodInfo mi;
                mi.name = method["name"];
                mi.obfuscated = method["obfuscated"];
                mi.signature = method["signature"];
                mi.isStatic = method["static"];
                ci.methods.push_back(mi);
            }

            result[class_name] = ci;
        }
        return result;
        }();

}  // namespace lunar_mappings
