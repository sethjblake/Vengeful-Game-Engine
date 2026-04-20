#include "Actor.h"
#include "LuaManager/LuaState.h"
#include "GlobalActorsMap.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <string>
#include "LuaManager/LuaActorHelper.h"
#include "Physics/GlobalPhysicsSystem.h"
#include "GlobalComponentCounter.h"
#include "Particles/GlobalParticleSystemsManager.h"

std::string VengefulGameEngine::Actor::GetName()
{
    return name;
}

int VengefulGameEngine::Actor::GetID()
{
    return uniqueActorID;
}

void VengefulGameEngine::Actor::injectLuaReferences(std::shared_ptr<luabridge::LuaRef> componentRef)
{
    (*componentRef)["actor"] = this;
}

luabridge::LuaRef VengefulGameEngine::Actor::GetComponentByKey(std::string key)
{
    if (components.find(key) == components.end()) {
        return luabridge::LuaRef(luaState);
    }

    luabridge::LuaRef rigidbodyRef = (*components[key]->luaRef)["Rigidbody"];

    if (!rigidbodyRef.isNil()) {
        return rigidbodyRef;
    }

    luabridge::LuaRef particleSystemRef = (*components[key]->luaRef)["ParticleSystem"];

    if (!particleSystemRef.isNil()) {
        return particleSystemRef;
    }

    return *components[key]->luaRef;
}

luabridge::LuaRef VengefulGameEngine::Actor::GetComponent(std::string typeName)
{
    for (auto& component : components) {
        if (component.second->type == typeName) {

            if (!(*component.second->luaRef)["enabled"]) {
                return luabridge::LuaRef(luaState);
            }

            luabridge::LuaRef rigidbodyRef = (*component.second->luaRef)["Rigidbody"];

            if (!rigidbodyRef.isNil()) {
                rigidbodyRef["actor"] = this;
                rigidbodyRef["key"] = component.second->key;
                return rigidbodyRef;
            }

            luabridge::LuaRef particleSystemRef = (*component.second->luaRef)["ParticleSystem"];

            if (!particleSystemRef.isNil()) {
                particleSystemRef["actor"] = this;
                particleSystemRef["key"] = component.second->key;
                return particleSystemRef;
            }

            return *component.second->luaRef;
        }
    }

    return luabridge::LuaRef(luaState);
}

luabridge::LuaRef VengefulGameEngine::Actor::GetComponents(std::string typeName)
{

    luabridge::LuaRef componentsTable = luabridge::LuaRef::newTable(luaState);

    int luaIndex{ 1 };

    for (auto& component : components) {
        if (component.second->type == typeName) {

            luabridge::LuaRef& luaRef = *component.second->luaRef;
            componentsTable[luaIndex] = luaRef;
            luaIndex++;
        }
    }

    return componentsTable;
}

luabridge::LuaRef VengefulGameEngine::Actor::AddComponent(std::string typeName)
{
    std::string key = "r" + std::to_string(globalAddComponentCallCount);
    globalAddComponentCallCount++;

    std::shared_ptr<luabridge::LuaRef> tablePointer = std::make_shared<luabridge::LuaRef>(luabridge::newTable(luaState));
    luabridge::LuaRef& newTable = *tablePointer;

    newTable["uniqueComponentID"] = globalComponentCounter;

    if (typeName == "Rigidbody") {
        newTable["Rigidbody"] = globalPhysicsSystem.createRigidbody();
    }
    else if (typeName == "ParticleSystem") {
        newTable["ParticleSystem"] = globalParticleSystemsManager.createParticleSystem(globalComponentCounter, uniqueActorID, key);
    }
    else {
        luabridge::LuaRef newMetatable = luabridge::newTable(luaState);
        luabridge::LuaRef templateLuaRef = luabridge::getGlobal(luaState, typeName.c_str());

        if (templateLuaRef.isNil()) {
            std::cout << "error: failed to locate component " << typeName;
            exit(0);
        }

        newMetatable["__index"] = templateLuaRef;
        newTable.push(luaState);
        newMetatable.push(luaState);
        lua_setmetatable(luaState, -2);
        lua_pop(luaState, 1);
    }
    
    globalComponentCounter++;

    newTable["key"] = key;
    newTable["enabled"] = true;

    std::shared_ptr<Component> newComponent = std::make_shared<Component>();

    newComponent->key = key;
    newComponent->isEnabled = true;
    newComponent->luaRef = tablePointer;
    newComponent->type = typeName;

    newComponentsQueue.push_back(std::pair<int, std::shared_ptr<Component>>(uniqueActorID, newComponent));

    injectLuaReferences(newComponent->luaRef);

    luabridge::LuaRef rigidbodyRef = newTable["Rigidbody"];

    if (!rigidbodyRef.isNil()) {
        rigidbodyRef["actor"] = this;
        rigidbodyRef["key"] = key;
        return rigidbodyRef;
    }

    return newTable;
}

void VengefulGameEngine::Actor::RemoveComponent(luabridge::LuaRef component)
{
    component["enabled"] = false;
    Actor* actor = component["actor"];
    std::shared_ptr<Component>& componentPtr = actor->components[component["key"]];

    luabridge::LuaRef particleSystemRef = (*componentPtr->luaRef)["ParticleSystem"];

    if (!particleSystemRef.isNil()) {
        particleSystemRef["enabled"] = false;
    }

    removeComponentQueue.push_back(std::pair<int, std::shared_ptr<Component>>(uniqueActorID, componentPtr));
}

void VengefulGameEngine::Actor::callLuaFunctionOnComponents(std::string functionName, bool ignoreDisabled)
{
    for (auto& item : components) {
        std::shared_ptr<Component> component = item.second;

        std::shared_ptr<luabridge::LuaRef> rigidbodyRef = std::make_shared<luabridge::LuaRef>((*component->luaRef)["Rigidbody"]);

        if (!(*rigidbodyRef).isNil()) {
            safeCallLuaFunction(name, functionName, rigidbodyRef, ignoreDisabled);
            continue;
        }

        safeCallLuaFunction(name, functionName, component->luaRef, ignoreDisabled);
    }
}
