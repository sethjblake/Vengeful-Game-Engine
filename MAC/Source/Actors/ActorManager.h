#ifndef ACTOR_MANAGER_H
#define ACTOR_MANAGER_H

#include <map>
#include <set>
#include "Actor.h"
#include "Lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"

namespace VengefulGameEngine {
	class ActorManager {
	public:
		std::map<int, std::set<std::string>> onStartActorKeySets;
		std::map<int, std::set<std::string>> onUpdateActorKeySets;
		std::map<int, std::set<std::string>> lateUpdateActorKeySets;
		void clearAll();
		~ActorManager();
	};
}

#endif
