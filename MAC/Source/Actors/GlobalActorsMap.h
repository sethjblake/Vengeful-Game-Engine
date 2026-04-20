#ifndef GLOBAL_ACTORS_MAP_H
#define GLOBAL_ACTORS_MAP_H

#include <map>
#include <vector>
#include <utility>
#include <memory>
#include "Actor.h"

using UNIQUE_ID = int;

namespace VengefulGameEngine {
	
	inline int globalActorsCounter = 0;

	inline int globalAddComponentCallCount = 0;

	inline std::map<UNIQUE_ID, Actor*> globalActorsMap;

	inline std::vector<std::pair<int, std::shared_ptr<Component>>> newComponentsQueue;

	inline std::vector<std::pair<int, std::shared_ptr<Component>>> removeComponentQueue;

	inline std::vector<Actor*> newActorsQueue;

	inline std::vector<Actor*> destroyActorsQueue;

}

#endif