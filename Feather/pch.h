#pragma once

#include <Windows.h>
#include <jni.h>
#include <jvmti.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <iostream>

#include <gl/GL.h>
#include <gl/GLU.h>

#include "MinHook.h"
#include "core/FeatherClient.h"
#include "core/ModuleManager.h"
#include "hooks/LunarClientHook.h"