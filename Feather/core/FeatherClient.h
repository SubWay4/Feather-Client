
#pragma once
#include <vector>
#include "../modules/ReachModule.h"
#include "../modules/ESPModule.h"

class FeatherClient {
public:
    FeatherClient();
    ~FeatherClient();

    static void initialize(LPVOID lpParam);
    void update(JNIEnv* env, jobject player, jobject minecraft);
    void render(JNIEnv* env, jobject minecraft);

private:
    std::vector<Module*> modules;
    ReachModule* reachModule;
    ESPModule* espModule;
};
