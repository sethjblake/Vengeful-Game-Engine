#ifndef LUA_MANAGER_H
#define LUA_MANAGER_H

#include <unordered_map>
#include "Lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"

namespace VengefulGameEngine {
	class LuaManager {
	public:
		void init();
		void reportError(const std::string& actorName, const luabridge::LuaException& e);
		~LuaManager();
	};
}

#endif