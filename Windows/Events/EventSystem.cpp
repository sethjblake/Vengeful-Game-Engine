#include "EventSystem.h"
#include "Lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"
#include <map>
#include <string>
#include <memory>
#include <algorithm>
#include "Actors/Actor.h"

void VengefulGameEngine::EventSystem::createSubscription(std::string eventType)
{
	eventNameToIDMap[eventType] = eventTypeCounter;
	eventTypeCounter++;
}

void VengefulGameEngine::EventSystem::createFirstSubscriber(std::string eventType, std::shared_ptr<luabridge::LuaRef> luaTable, std::shared_ptr<luabridge::LuaRef> luaFunction)
{
	createSubscription(eventType);

	EventTable eventTable;
	eventTable.luaFunction = luaFunction;
	eventTable.luaTable = luaTable;

	EventType newEventType;

	int tableID = newEventType.subscriberCounter;
	newEventType.eventTables[tableID] = eventTable;

	int componentID = (*luaTable)["uniqueComponentID"];
	newEventType.componentIDToTableIDMap[componentID] = tableID;

	newEventType.subscriberCounter++;

	int id = getEventID(eventType);
	eventIDToTypeMap[id] = newEventType;
	
}

int VengefulGameEngine::EventSystem::getEventID(std::string eventType)
{
	if (eventNameToIDMap.find(eventType) == eventNameToIDMap.end()) {
		return -1;
	}

	return eventNameToIDMap[eventType];
}

void VengefulGameEngine::EventSystem::addSubscriber(std::string eventType, std::shared_ptr<luabridge::LuaRef> luaTable, std::shared_ptr<luabridge::LuaRef> luaFunction)
{
	if (eventNameToIDMap.find(eventType) == eventNameToIDMap.end()) {
		createFirstSubscriber(eventType, luaTable, luaFunction);
		return;
	}

	int id = getEventID(eventType);
	EventType& existingEventType = eventIDToTypeMap[id];
	
	int tableID = existingEventType.subscriberCounter;
	int componentID = (*luaTable)["uniqueComponentID"];
	existingEventType.componentIDToTableIDMap[componentID] = tableID;

	EventTable& existingEventTable = existingEventType.eventTables[tableID];
	existingEventTable.luaTable = luaTable;
	existingEventTable.luaFunction = luaFunction;
	
	existingEventType.subscriberCounter++;

}

void VengefulGameEngine::EventSystem::removeSubscriber(std::string eventType, std::shared_ptr<luabridge::LuaRef> luaTable, std::shared_ptr<luabridge::LuaRef> luaFunction)
{
	if (eventNameToIDMap.find(eventType) == eventNameToIDMap.end()) {
		return;
	}
	int id = eventNameToIDMap[eventType];
	EventType& type = eventIDToTypeMap[id];

	int uniqueIDToRemove = (*luaTable)["uniqueComponentID"];
	if (type.componentIDToTableIDMap.find(uniqueIDToRemove) == type.componentIDToTableIDMap.end()) {
		return;
	}
	
	int tableID = type.componentIDToTableIDMap[uniqueIDToRemove];

	type.eventTables.erase(tableID);
	type.componentIDToTableIDMap.erase(uniqueIDToRemove);
}

void VengefulGameEngine::EventSystem::publishEvent(std::string eventType, luabridge::LuaRef& eventObject)
{
	if (eventNameToIDMap.find(eventType) == eventNameToIDMap.end()) {
		return;
	}

	int id = eventNameToIDMap[eventType];

	EventType& type = eventIDToTypeMap[id];

	for (auto& item : type.eventTables) {
		EventTable& table = item.second;
		luabridge::LuaRef luaFunction = *table.luaFunction;
		luabridge::LuaRef luaTable = *table.luaTable;
		try {
			luaFunction(luaTable, eventObject);
		}
		catch (const luabridge::LuaException& e) {
			std::string errorMessage = e.what();
			std::replace(errorMessage.begin(), errorMessage.end(), '\\', '/');
			Actor* actor = luaTable["actor"];
			std::string actorName;
			if (actor) actorName = actor->name;
			std::cout << "\033[31m" << actorName << " : " << errorMessage << "\033[0m" << std::endl;
		}
	}

}

void VengefulGameEngine::EventSystem::updateSubscriptions()
{
	for (auto& request : newSubscribersQueue) {
		addSubscriber(request.eventType, request.luaTable, request.luaFunction);
	}
	for (auto& request : removeSubscribersQueue) {
		removeSubscriber(request.eventType, request.luaTable, request.luaFunction);
	}
	newSubscribersQueue.clear();
	removeSubscribersQueue.clear();
}
