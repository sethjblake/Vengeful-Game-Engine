#ifndef EVENT_SYSTEM_H
#define EVENT_SYSTEM_H

#include "Lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"

#include <map>
#include <string>
#include <memory>
#include "SubscriptionRequest.h"

namespace VengefulGameEngine {

	struct EventTable {
		std::shared_ptr<luabridge::LuaRef> luaFunction;
		std::shared_ptr<luabridge::LuaRef> luaTable;
	};


	struct EventType {
		std::map<int, EventTable> eventTables;
		std::map<int, int> componentIDToTableIDMap;
		int subscriberCounter = 0;
	};

	class EventSystem {
	public:

		void createSubscription(std::string eventType);

		void createFirstSubscriber(std::string eventType, std::shared_ptr<luabridge::LuaRef> luaTable, std::shared_ptr<luabridge::LuaRef> luaFunction);

		int getEventID(std::string eventType);

		void addSubscriber(std::string eventType, std::shared_ptr<luabridge::LuaRef> luaTable, std::shared_ptr<luabridge::LuaRef> luaFunction);

		void removeSubscriber(std::string eventType, std::shared_ptr<luabridge::LuaRef> luaTable, std::shared_ptr<luabridge::LuaRef> luaFunction);

		void publishEvent(std::string eventType, luabridge::LuaRef& eventObject);

		void updateSubscriptions();

		int eventTypeCounter = 0;
		std::map<std::string, int> eventNameToIDMap;
		std::map<int, EventType> eventIDToTypeMap;

		std::vector<SubscriptionRequest> newSubscribersQueue;
		std::vector<SubscriptionRequest> removeSubscribersQueue;
	};
}

#endif