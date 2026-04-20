#include "SceneManager.h"
#include <iostream>
#include <utility>
#include "rapidjson/document.h"
#include "Actors/Actor.h"
#include "Actors/Component.h"
#include "Actors/ActorManager.h"
#include "Lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"
#include "LuaManager/LuaManager.h"
#include "LuaManager/LuaState.h"
#include "Actors/GlobalActorsMap.h"
#include <queue>
#include "Physics/Rigidbody.h"
#include "Physics/RigidbodyConstants.h"
#include "Physics/GlobalPhysicsSystem.h"
#include "EngineConfig/Macros.h"
#include "LuaManager/LuaActorInterface.h"

void VengefulGameEngine::SceneManager::loadNewScene(std::string sceneName, ActorManager& actorManager, ContentLoader& contentLoader, LuaManager& luaManager)
{
	std::string resourcesFolder = RESOURCES_FOLDER;
	std::string filePath = resourcesFolder + "/scenes/";
	std::string fileName = sceneName + ".scene";
	assertSceneExists(sceneName, filePath, fileName, contentLoader);
	rapidjson::Document document;
	contentLoader.loadRapidJSON(document, filePath, fileName);

	std::map<int, Actor*>& actors = globalActorsMap;

	std::string key;
	key = "actors";

	if (!document.HasMember(key.c_str()) || !document[key.c_str()].IsArray()) {
		return;
	}

	rapidjson::GenericArray arr = document[key.c_str()].GetArray();

	for (int i = 0; i < arr.Size(); i++) {
		Actor* actor = CreateActorFromDocumentArray(document, i);
		actors[actor->uniqueActorID] = actor;
		injectReferencesToSelf(actor);
		queueOnStart(actor, actorManager);
	}

	queueUpdateLogic(actorManager);
}

void VengefulGameEngine::SceneManager::assertSceneExists(std::string& sceneName, std::string& filePath, std::string& fileName, ContentLoader& contentLoader)
{
	if (!contentLoader.filePathExists(filePath)) {
		std::cout << "error: " << filePath << " missing";
	}

	if (!contentLoader.fileExists(filePath, fileName)) {
		std::cout << "error: " << sceneName << " missing";
	}
}

void VengefulGameEngine::SceneManager::queueOnStart(Actor* actor, ActorManager& actorManager)
{
	std::map<std::string, std::shared_ptr<Component>>& components = actor->components;

	for (auto& component : components) {
		std::shared_ptr<luabridge::LuaRef>& luaRefPtr = component.second->luaRef;
		luabridge::LuaRef& luaRef = *luaRefPtr;

		if (!luaRef.isTable()) {
			std::cout << "error: luaRef is invalid";
			delete actor;
			exit(0);
		}

		luabridge::LuaRef function = luaRef["OnStart"];
		
		bool isNil = function.isNil();

		if (isNil) continue;

		bool isFuction = function.isFunction();

		if (!isFuction) continue;

		actorManager.onStartActorKeySets[actor->uniqueActorID].insert(component.second->key);

	}
}

void VengefulGameEngine::SceneManager::injectReferencesToSelf(Actor* actor)
{
	std::map<std::string, std::shared_ptr<Component>>& components = actor->components;
	for (auto& component : components) {
		actor->injectLuaReferences(component.second->luaRef);
	}
}

void VengefulGameEngine::SceneManager::queueUpdateLogic(ActorManager& actorManager)
{
	for (auto& item : globalActorsMap) {
		Actor* actor = item.second;

		std::map<std::string, std::shared_ptr<Component>>& components = actor->components;

		for (auto& component : components) {
			std::shared_ptr<luabridge::LuaRef>& luaRefPtr = component.second->luaRef;
			luabridge::LuaRef& luaRef = *luaRefPtr;

			if (!luaRef.isTable()) {
				std::cout << "error: luaRef is invalid";
				delete actor;
				exit(0);
			}

			luabridge::LuaRef updateFunction = luaRef["OnUpdate"];

			if (!updateFunction.isNil() && updateFunction.isFunction()) {
				actorManager.onUpdateActorKeySets[actor->uniqueActorID].insert(component.second->key);
			}

			luabridge::LuaRef lateUpdateFunction = luaRef["OnLateUpdate"];

			if (!lateUpdateFunction.isNil() && lateUpdateFunction.isFunction()) {
				actorManager.lateUpdateActorKeySets[actor->uniqueActorID].insert(component.second->key);
			}

		}
	}
}

void VengefulGameEngine::SceneManager::clearScene(ActorManager& actorManager)
{
	std::queue<Actor*> dontDestroyOnLoad;

	for (auto& item : globalActorsMap) {

		if (item.second == nullptr) {
			continue;
		}

		if (item.second->dontDestroyOnLoad) {
			dontDestroyOnLoad.push(item.second);
		}
		else {
			int id = item.second->uniqueActorID;
			globalActorsMap[id]->callLuaFunctionOnComponents("OnDestroy", false);
			actorManager.onStartActorKeySets.erase(id);
			actorManager.onUpdateActorKeySets.erase(id);
			actorManager.lateUpdateActorKeySets.erase(id);
			delete item.second;
			item.second = nullptr;
		}
	}

	globalActorsMap.clear();

	while (!dontDestroyOnLoad.empty()) {
		Actor* actor = dontDestroyOnLoad.front();
		globalActorsMap[actor->uniqueActorID] = actor;
		dontDestroyOnLoad.pop();
	}
}

std::shared_ptr<luabridge::LuaRef> VengefulGameEngine::SceneManager::createComponent(std::string type)
{
	std::shared_ptr<luabridge::LuaRef> tablePointer =
		std::make_shared<luabridge::LuaRef>(luabridge::newTable(luaState));

	if (type == "Rigidbody") {
		Rigidbody* rigidbody = globalPhysicsSystem.createRigidbody();
		(*tablePointer)["Rigidbody"] = rigidbody;
	}

	luabridge::LuaRef& newTable = *tablePointer;
	luabridge::LuaRef newMetatable = luabridge::newTable(luaState);
	luabridge::LuaRef templateLuaRef = luabridge::getGlobal(luaState, type.c_str());

	if (templateLuaRef.isNil()) {
		std::cout << "error: failed to locate component " << type;
		exit(0);
	}

	newMetatable["__index"] = templateLuaRef;
	newTable.push(luaState);
	newMetatable.push(luaState);
	lua_setmetatable(luaState, -2);
	lua_pop(luaState, 1);

	return tablePointer;
}
