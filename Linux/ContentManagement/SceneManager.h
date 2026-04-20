#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include <string>
#include <map>
#include "ContentManagement/ContentLoader.h"
#include "rapidjson/include/rapidjson/document.h"
#include "Actors/Actor.h"
#include "Actors/ActorManager.h"
#include "LuaManager/LuaManager.h"
#include "EngineConfig/Macros.h"

namespace VengefulGameEngine {
	class SceneManager {
	public:
		void loadNewScene(std::string sceneName, ActorManager& actorManager, ContentLoader& contentLoader, LuaManager& luaManager);
		void assertSceneExists(std::string& sceneName, std::string& filePath, std::string& fileName, ContentLoader& contentLoader);
		void loadActorFromTemplate(Actor* actor, std::string actorTemplate, ContentLoader& contentLoader, LuaManager& luaManager);
		void queueOnStart(Actor* actor, ActorManager& actorManager);
		void injectReferencesToSelf(Actor* actor);
		void queueUpdateLogic(ActorManager& actorManager);
		void clearScene(ActorManager& actorManager);
		std::shared_ptr<luabridge::LuaRef> createComponent(std::string type);
	private:
		std::string currentScene;
	};
}

#endif