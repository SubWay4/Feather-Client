#pragma once
#include <vector>
#include <memory>
#include "../modules/ReachModule.h"
#include "../modules/ESPModule.h"

class FeatherClient {
public:
    FeatherClient();
    ~FeatherClient();

    void initialize(LPVOID lpParam, const std::string& version);
    void update(JNIEnv* env, jobject player, jobject minecraft);
    void render(JNIEnv* env, jobject minecraft);

private:
    std::vector<std::unique_ptr<Module>> modules;
    ReachModule* reachModule;  // Raw pointer for quick access, owned by modules vector
    ESPModule* espModule;      // Raw pointer for quick access, owned by modules vector
};