#ifndef LUA_MANAGER_H
#define LUA_MANAGER_H

#include <unordered_map>
#include "../../ThirdParty/Lua/Lua.hpp"
#include "../../ThirdParty/LuaBridge/LuaBridge.h"

namespace VengefulGameEngine {
	class LuaManager {
	public:
		void init();
		void reportError(const std::string& actorName, const luabridge::LuaException& e);
		void closeLua();
	};
}

#endif