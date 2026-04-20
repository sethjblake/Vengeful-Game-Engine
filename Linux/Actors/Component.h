#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <string>
#include <memory>

#include "Lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"

namespace VengefulGameEngine {
	
	class Component {
	public:
		
		std::string key;
		
		std::string type;

		std::shared_ptr<luabridge::LuaRef> luaRef = nullptr;

		bool isEnabled = true;

	};
}

#endif
