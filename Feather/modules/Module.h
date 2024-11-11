#pragma once
#include "pch.h"


class Module {
public:
    Module(const std::string& name) : name(name), enabled(false) {}
    virtual ~Module() = default;

    virtual void onEnable() {}
    virtual void onDisable() {}
    virtual void onUpdate(JNIEnv* env, jobject player) {}
    virtual void onRender(JNIEnv* env, jobject minecraft) {}

    bool isEnabled() const { return enabled; }
    void toggle() { enabled = !enabled; }

protected:
    std::string name;
    bool enabled;
};