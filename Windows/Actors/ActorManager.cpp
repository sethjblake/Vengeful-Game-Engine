#include "ActorManager.h"
#include "GlobalActorsMap.h"

void VengefulGameEngine::ActorManager::clearAll()
{
	for (auto& item : globalActorsMap) {
		delete item.second;
	}
	globalActorsMap.clear();
	onStartActorKeySets.clear();
}

VengefulGameEngine::ActorManager::~ActorManager()
{
	clearAll();
}
