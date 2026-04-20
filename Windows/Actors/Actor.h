#ifndef ACTOR_H
#define ACTOR_H

#include <string>
#include <map>
#include <memory>
#include "Component.h"

#include "../../ThirdParty/Lua/Lua.hpp"
#include "../../ThirdParty/LuaBridge/LuaBridge.h"

namespace VengefulGameEngine {
	class Actor {

	public:

		int uniqueActorID = -1;

		std::string name = "";
		
		std::string templateName = "";

		std::map<std::string, std::shared_ptr<Component>> components;

		std::shared_ptr<luabridge::LuaRef> selfLuaRef;

		std::string GetName();

		int GetID();

		void injectLuaReferences(std::shared_ptr<luabridge::LuaRef> componentRef);

		luabridge::LuaRef GetComponentByKey(std::string key);

		luabridge::LuaRef GetComponent(std::string typeName);

		luabridge::LuaRef GetComponents(std::string typeName);

		luabridge::LuaRef AddComponent(std::string typeName);

		void RemoveComponent(luabridge::LuaRef component);

		bool dontDestroyOnLoad = false;

		void callLuaFunctionOnComponents(std::string functionName, bool ignoreDisabled);

	};
}

#endif
