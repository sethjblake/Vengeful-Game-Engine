#ifndef EVENT_BUS_H
#define EVENT_BUS_H

#include "Lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"

namespace VengefulGameEngineEvents {
	
	void Subscribe(std::string eventType, luabridge::LuaRef component, luabridge::LuaRef luaFunction);

	void Unsubscribe(std::string eventType, luabridge::LuaRef component, luabridge::LuaRef luaFunction);

	void Publish(std::string eventType, luabridge::LuaRef eventObject);
}

#endif