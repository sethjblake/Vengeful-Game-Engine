#ifndef LUA_ACTOR_HELPER_H
#define LUA_ACTOR_HELPER_H

#include <string>
#include <memory>
#include <algorithm>
#include "Lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"
#include "Actors/Component.h"

namespace VengefulGameEngine {
    void safeCallLuaFunction(std::string& actorName, std::string& functionName, std::shared_ptr<luabridge::LuaRef> luaRef, bool ignoreIfDisabled);
}

#endif