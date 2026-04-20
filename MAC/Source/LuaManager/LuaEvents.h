#ifndef LUA_EVENTS_H
#define LUA_EVENTS_H

#include "Lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"
#include <string>
#include "Events/GlobalEventSystem.h"
#include "Events/SubscriptionRequest.h"
#include <memory>

namespace VengefulGameEngine {
	
	void Subscribe(std::string eventType, luabridge::LuaRef component, luabridge::LuaRef luaFunction) {
		SubscriptionRequest request;
		request.eventType = eventType;
		request.luaTable = std::make_shared<luabridge::LuaRef>(component);
		request.luaFunction = std::make_shared<luabridge::LuaRef>(luaFunction);
		globalEventSystem.newSubscribersQueue.push_back(request);
	}

	void Unsubscribe(std::string eventType, luabridge::LuaRef component, luabridge::LuaRef luaFunction) {
		SubscriptionRequest request;
		request.eventType = eventType;
		request.luaTable = std::make_shared<luabridge::LuaRef>(component);
		request.luaFunction = std::make_shared<luabridge::LuaRef>(luaFunction);
		globalEventSystem.removeSubscribersQueue.push_back(request);
	}

	void Publish(std::string eventType, luabridge::LuaRef eventObject) {
		globalEventSystem.publishEvent(eventType, eventObject);
	}
}

#endif