#ifndef LUA_SCENE_H
#define LUA_SCENE_H

#include <string>
#include "../Actors/Actor.h"
#include "../ContentManagement/GlobalSceneData.h"
#include "../Actors/GlobalActorsMap.h"

using VengefulGameEngine::Actor;

namespace LuaScene {
	
	void Load(std::string sceneName) {
		VengefulGameEngine::globalSceneData.newSceneName = sceneName;
	}

	std::string GetCurrent() {
		return VengefulGameEngine::globalSceneData.currentScene;
	}

	void DontDestroy(Actor* actor) {
		VengefulGameEngine::globalActorsMap[actor->uniqueActorID]->dontDestroyOnLoad = true;
	}
}

#endif