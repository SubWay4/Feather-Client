#pragma once
#include "Module.h"

class ReachModule : public Module {
public:
    ReachModule();
    void onUpdate(JNIEnv* env, jobject player) override;
};