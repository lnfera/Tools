#pragma once
#pragma message("-------Never give up on your dreams!-------------")
#include "targetver.h"
#include <tge/error/ErrorManager.h>

#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN
#endif

#include <tge/math/matrix4x4.h>
#include <tge/math/vector2.h>
#include <tge/math/vector4.h>
#include <tge/sprite/sprite.h>
#include <tge/shaders/shader.h>
#include <tge/engine.h>

#include <tge/EngineUtilities.h>
#include <tge/settings/settings.h>

#include <algorithm>
#include <array>
#include <exception>
#include <malloc.h>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>
#include <cassert>

#include "imgui/imgui.h"

//#define FMT_UNICODE 0
//#include <spdlog/spdlog.h>
