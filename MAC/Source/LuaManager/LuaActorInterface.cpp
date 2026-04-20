#include "LuaActorInterface.h"
#include "Lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"
#include "Actors/ActorManager.h"
#include "Actors/Actor.h"
#include "Actors/GlobalActorsMap.h"
#include "LuaState.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include <iostream>
#include <filesystem>
#include <cstdlib>
#include "EngineConfig/Macros.h"
#include "Physics/Rigidbody.h"
#include "Physics/RigidbodyConstants.h"
#include "Physics/GlobalPhysicsSystem.h"
#include "Actors/GlobalComponentCounter.h"
#include "Particles/ParticleSystem.h"
#include "Particles/GlobalParticleSystemsManager.h"


std::shared_ptr<luabridge::LuaRef> VengefulGameEngine::createComponentOfType(std::string type, int actorID, std::string key)
{
	std::shared_ptr<luabridge::LuaRef> tablePointer =
		std::make_shared<luabridge::LuaRef>(luabridge::newTable(luaState));

	if (type == "Rigidbody") {
		Rigidbody* rigidbody = globalPhysicsSystem.createRigidbody();
		(*tablePointer)["Rigidbody"] = rigidbody;
		(*tablePointer)["uniqueComponentID"] = globalComponentCounter++;
		return tablePointer;
	}

	if (type == "ParticleSystem") {
		ParticleSystem* particleSystem = globalParticleSystemsManager.createParticleSystem(globalComponentCounter, actorID, key);
		(*tablePointer)["ParticleSystem"] = particleSystem;
		(*tablePointer)["uniqueComponentID"] = globalComponentCounter++;
		return tablePointer;
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

	(*tablePointer)["uniqueComponentID"] = globalComponentCounter++;

	return tablePointer;
}

VengefulGameEngine::Actor* VengefulGameEngine::Find(std::string name) {

	for (auto& item : destroyActorsQueue) {
		if (item->name == name) {
			return nullptr;
		}
	}

	for (auto& item : globalActorsMap) {
		if (item.second->name == name) {
			Actor*& actor = item.second;
			return actor;
		}
	}

	for (Actor*& actor : newActorsQueue) {
		if (actor->name == name) {
			return actor;
		}
	}

	return nullptr;
}

luabridge::LuaRef VengefulGameEngine::FindAll(std::string name) {

	luabridge::LuaRef actorsTable = luabridge::LuaRef::newTable(luaState);

	int luaIndex{ 1 };

	for (auto& item : globalActorsMap) {
		Actor* actor = item.second;
		if (actor->name == name) {
			actorsTable[luaIndex] = actor;
			luaIndex++;
		}
	}

	for (Actor*& actor : newActorsQueue) {
		if (actor->name == name) {
			actorsTable[luaIndex] = actor;
			luaIndex++;
		}
	}

	return actorsTable;
}

void VengefulGameEngine::LoadActorTemplate(std::string actorTemplateName, rapidjson::Document& document) {
	std::string resourcesFolder = RESOURCES_FOLDER;

	std::string path = resourcesFolder + "/actor_templates";

	std::string fileName = actorTemplateName + ".template";

	FILE* file_pointer = nullptr;
	std::string filePath = path + "/" + fileName;

	if (!std::filesystem::exists(filePath)) {
		std::cout << "error: " + filePath + " missing";
		exit(0);
	}

#ifdef _WIN32
	fopen_s(&file_pointer, filePath.c_str(), "rb");
#else
	file_pointer = fopen(filePath.c_str(), "rb");
#endif
	char buffer[65536];
	rapidjson::FileReadStream stream(file_pointer, buffer, sizeof(buffer));
	document.ParseStream(stream);
	std::fclose(file_pointer);

	if (document.HasParseError()) {
		rapidjson::ParseErrorCode errorCode = document.GetParseError();
		std::cout << "error parsing json at [resources]" << std::endl;
		exit(0);
	}
}

VengefulGameEngine::Actor* VengefulGameEngine::CreateActorFromDocument(rapidjson::Document& document) {

	Actor* actor = new Actor();

	actor->uniqueActorID = globalActorsCounter;
	globalActorsCounter++;

	std::string key;

	key = "name";
	if (document.HasMember(key.c_str())) {
		actor->name = document[key.c_str()].GetString();
	}

	key = "components";
	if (!document.HasMember(key.c_str())) {
		newActorsQueue.push_back(actor);
		return actor;
	}

	rapidjson::GenericObject componentObjects = document[key.c_str()].GetObject();

	for (auto& componentObject : componentObjects) {

		std::shared_ptr<Component> component = std::make_shared<Component>();

		component->key = componentObject.name.GetString();

		rapidjson::GenericObject paramtersObject = componentObject.value.GetObject();


		key = "type";
		if (paramtersObject.HasMember(key.c_str())) {
			component->type = paramtersObject[key.c_str()].GetString();
		}

		std::shared_ptr<luabridge::LuaRef> tablePointer = createComponentOfType(component->type, actor->uniqueActorID, component->key);
		luabridge::LuaRef& newTable = *tablePointer;

		luabridge::LuaRef rigidbodyRef = newTable["Rigidbody"];
		bool isRigidbody = !rigidbodyRef.isNil();

		if (isRigidbody) {
			rigidbodyRef[globalRigidbodyConstants.PARENT_ACTOR_ID.c_str()] = actor->uniqueActorID;
			rigidbodyRef["actor"] = actor;
			rigidbodyRef["key"] = component->key;
			rigidbodyRef["enabled"] = true;
		}

		RigidbodyConstants rigidbodyConstants;

		luabridge::LuaRef particleSystemRef = newTable["ParticleSystem"];
		bool isParticleSystem = !particleSystemRef.isNil();

		if (isParticleSystem) {
			particleSystemRef["actor"] = actor;
			particleSystemRef["key"] = component->key;
			particleSystemRef["enabled"] = true;
		}

		for (auto& parameterObject : paramtersObject) {
			std::string parameterName = parameterObject.name.GetString();

			if (parameterName == "type") {
				continue;
			}
			else if (parameterObject.value.IsString()) {
				if (isRigidbody) {
					rigidbodyRef[parameterName] = parameterObject.value.GetString();
				}
				else if (isParticleSystem) {
					particleSystemRef[parameterName] = parameterObject.value.GetString();
				}
				else {
					newTable[parameterName] = parameterObject.value.GetString();
				}
			}
			else if (parameterObject.value.IsFloat()) {
				if (isRigidbody) {
					rigidbodyRef[parameterName] = parameterObject.value.GetFloat();
				}
				else if (isParticleSystem) {
					particleSystemRef[parameterName] = parameterObject.value.GetFloat();
				}
				else {
					newTable[parameterName] = parameterObject.value.GetFloat();
				}
			}
			else if (parameterObject.value.IsInt()) {
				if (isRigidbody) {
					rigidbodyRef[parameterName] = parameterObject.value.GetInt();
				}
				else if (isParticleSystem) {
					particleSystemRef[parameterName] = parameterObject.value.GetInt();
				}
				else {
					newTable[parameterName] = parameterObject.value.GetInt();
				}
			}
			else if (parameterObject.value.IsBool()) {
				if (isRigidbody) {
					rigidbodyRef[parameterName] = parameterObject.value.GetBool();
				}
				else if (isParticleSystem) {
					particleSystemRef[parameterName] = parameterObject.value.GetBool();
				}
				else {
					newTable[parameterName] = parameterObject.value.GetBool();
				}
			}
		}

		newTable["key"] = component->key;
		newTable["enabled"] = true;
		component->luaRef = tablePointer;
		actor->injectLuaReferences(tablePointer);
		actor->components[component->key] = component;
	}

	return actor;
}

VengefulGameEngine::Actor* VengefulGameEngine::CreateActorFromDocumentArray(rapidjson::Document& document, int index)
{
	std::string key;
	key = "actors";
	if (!document.HasMember(key.c_str()) || !document[key.c_str()].IsArray()) {
		return nullptr;
	}
	rapidjson::GenericArray arr = document[key.c_str()].GetArray();

	Actor* actor = new Actor();

	actor->uniqueActorID = globalActorsCounter;
	globalActorsCounter++;

	key = "name";
	if (arr[index].HasMember(key.c_str())) {
		actor->name = arr[index][key.c_str()].GetString();
	}

	key = "template";
	if (arr[index].HasMember(key.c_str())) {
		std::string templateName = arr[index][key.c_str()].GetString();
		actor = CreateActor(templateName);
	}

	key = "components";
	if (!arr[index].HasMember(key.c_str())) {
		newActorsQueue.push_back(actor);
		return actor;
	}

	rapidjson::GenericObject componentObjects = arr[index][key.c_str()].GetObject();

	for (auto& componentObject : componentObjects) {

		std::shared_ptr<Component> component = std::make_shared<Component>();

		component->key = componentObject.name.GetString();

		rapidjson::GenericObject paramtersObject = componentObject.value.GetObject();


		key = "type";
		if (paramtersObject.HasMember(key.c_str())) {
			component->type = paramtersObject[key.c_str()].GetString();
		}

		std::shared_ptr<luabridge::LuaRef> tablePointer = createComponentOfType(component->type, actor->uniqueActorID, component->key);
		luabridge::LuaRef& newTable = *tablePointer;

		luabridge::LuaRef rigidbodyRef = newTable["Rigidbody"];
		bool isRigidbody = !rigidbodyRef.isNil();


		if (isRigidbody) {
			rigidbodyRef[globalRigidbodyConstants.PARENT_ACTOR_ID.c_str()] = actor->uniqueActorID;
			rigidbodyRef["actor"] = actor;
			rigidbodyRef["key"] = component->key;
			rigidbodyRef["enabled"] = true;
		}

		luabridge::LuaRef particleSystemRef = newTable["ParticleSystem"];
		bool isParticleSystem = !particleSystemRef.isNil();

		if (isParticleSystem) {
			particleSystemRef["actor"] = actor;
			particleSystemRef["key"] = component->key;
			particleSystemRef["enabled"] = true;
		}

		RigidbodyConstants rigidbodyConstants;

		for (auto& parameterObject : paramtersObject) {
			std::string parameterName = parameterObject.name.GetString();

			if (parameterName == "type") {
				continue;
			}
			else if (parameterObject.value.IsString()) {
				if (isRigidbody) {
					rigidbodyRef[parameterName] = parameterObject.value.GetString();
				}
				else if (isParticleSystem) {
					particleSystemRef[parameterName] = parameterObject.value.GetString();
				}
				else {
					newTable[parameterName] = parameterObject.value.GetString();
				}
			}
			else if (parameterObject.value.IsFloat()) {
				if (isRigidbody) {
					rigidbodyRef[parameterName] = parameterObject.value.GetFloat();
				}
				else if (isParticleSystem) {
					particleSystemRef[parameterName] = parameterObject.value.GetFloat();
				}
				else {
					newTable[parameterName] = parameterObject.value.GetFloat();
				}
			}
			else if (parameterObject.value.IsInt()) {
				if (isRigidbody) {
					rigidbodyRef[parameterName] = parameterObject.value.GetInt();
				}
				else if (isParticleSystem) {
					particleSystemRef[parameterName] = parameterObject.value.GetInt();
				}
				else {
					newTable[parameterName] = parameterObject.value.GetInt();
				}
			}
			else if (parameterObject.value.IsBool()) {
				if (isRigidbody) {
					rigidbodyRef[parameterName] = parameterObject.value.GetBool();
				}
				else if (isParticleSystem) {
					particleSystemRef[parameterName] = parameterObject.value.GetBool();
				}
				else {
					newTable[parameterName] = parameterObject.value.GetBool();
				}
			}
		}

		newTable["key"] = component->key;
		newTable["enabled"] = true;
		component->luaRef = tablePointer;
		actor->injectLuaReferences(tablePointer);
		actor->components[component->key] = component;
	}

	return actor;
}

VengefulGameEngine::Actor* VengefulGameEngine::CreateActor(std::string actorTemplateName)
{
	rapidjson::Document document;
	LoadActorTemplate(actorTemplateName, document);
	Actor* actor = CreateActorFromDocument(document);
	return actor;
}

VengefulGameEngine::Actor* VengefulGameEngine::Instantiate(std::string actorTemplateName) {
	Actor* actor = CreateActor(actorTemplateName);
	newActorsQueue.push_back(actor);
	return actor;
}

void VengefulGameEngine::Destroy(Actor* actor) {
	int id = actor->uniqueActorID;

	std::map<std::string, std::shared_ptr<Component>>& components = actor->components;

	for (auto& item : components) {
		luabridge::LuaRef& componentRef = *item.second->luaRef;
		actor->RemoveComponent(componentRef);
	}

	destroyActorsQueue.push_back(actor);
}
