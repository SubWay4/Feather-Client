#pragma once
#include "modules/Module.h"
#include <vector>

struct PlayerPosition {
    float x, y, z;
};

class ESPModule : public Module {
public:
    ESPModule();
    void onRender(JNIEnv* env, jobject minecraft) override;
    void updatePlayerPositions(JNIEnv* env, jobject minecraft);

private:
    std::vector<PlayerPosition> playerPositions;
};
