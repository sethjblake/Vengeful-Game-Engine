#ifndef LUA_ACTOR_INFO_H
#define LUA_ACTOR_INFO_H

#include "../../ThirdParty/Lua/lua.hpp"
#include "../../ThirdParty/LuaBridge/LuaBridge.h"
#include "../Actors/ActorManager.h"
#include "../Actors/Actor.h"
#include "../Actors/GlobalActorsMap.h"
#include "LuaState.h"
#include "../../ThirdParty/rapidjson/include/rapidjson/document.h"
#include "../../ThirdParty/rapidjson/include/rapidjson/filereadstream.h"
#include <iostream>
#include <filesystem>
#include <cstdlib>
#include "EngineConfig/Macros.h"
#include "Physics/Rigidbody.h"
#include "Physics/RigidbodyConstants.h"
#include "Physics/GlobalPhysicsSystem.h"

namespace VengefulGameEngine {
	std::shared_ptr<luabridge::LuaRef> createComponentOfType(std::string type, int actorID, std::string key);

	Actor* Find(std::string name);

	luabridge::LuaRef FindAll(std::string name);

	void LoadActorTemplate(std::string actorTemplateName, rapidjson::Document& document);
	
	Actor* CreateActorFromDocument(rapidjson::Document& document);

	Actor* CreateActorFromDocumentArray(rapidjson::Document& document, int index);

	Actor* CreateActorFromTemplate(std::string actorTemplateName);

	Actor* CreateEmptyActor();

	Actor* Instantiate(std::string actorTemplateName);

	void Destroy(Actor* actor);
}

#endif