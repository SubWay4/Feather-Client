#pragma once

#include <Windows.h>
#include <gl/GL.h>

class OpenGLHook {
public:
    static bool initialize();
    static void renderOverlay();
};
