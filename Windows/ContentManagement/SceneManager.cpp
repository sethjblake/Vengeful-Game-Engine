#include "SceneManager.h"
#include <iostream>
#include <utility>
#include "../../ThirdParty/rapidjson/include/rapidjson/document.h"
#include "../Actors/Actor.h"
#include "../Actors/Component.h"
#include "../Actors/ActorManager.h"
#include "../../ThirdParty/Lua/Lua.hpp"
#include "../../ThirdParty/LuaBridge/LuaBridge.h"
#include "../LuaManager/LuaManager.h"
#include "../LuaManager/LuaState.h"
#include "../Actors/GlobalActorsMap.h"
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
		queueUpdateLogic(actor, actorManager);
	}
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

void VengefulGameEngine::SceneManager::queueUpdateLogic(Actor* actor, ActorManager& actorManager)
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

		luabridge::LuaRef startFunction = luaRef["OnStart"];

		if (!startFunction.isNil() && startFunction.isFunction()) {
			actorManager.onStartActorKeySets[actor->uniqueActorID].insert(component.second->key);
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
