#ifndef GAME_2D_H
#define GAME_2D_H

#include "ContentManagement/ContentLoader.h"
#include "ContentManagement/DataBank.h"
#include "ContentManagement/GameConfigRegistry.h"
#include "ContentManagement/RenderConfigRegistry.h"
#include "Engine/Game.h"
#include "Graphics/WindowManager.h"
#include "Graphics/TextManager.h"
#include "LuaManager/LuaManager.h"
#include "ContentManagement/SceneManager.h"
#include "Actors/ActorManager.h"
#include "AudioSystem/AudioManager.h"
#include "Constants/GameConfigConstants.h"
#include "Constants/RenderingConfigConstants.h"

namespace VengefulGameEngine {

	class Game2D : public Game {

	public:
		void initBeforeFirstRender();

		bool isRunning();

		void readInput();

		void addComponents();

		void update();

		void render();

		void lateUpdate();

		void afterGameEnd();

		// Helpers

		void checkIfResourcesExists();

		void loadGameConfig();

		void loadRenderConfig();

		void initWindow();

		void initAudio();

		void initLua();

		void initPhysics();

		void loadNewScene(std::string sceneName);

		void removeComponents();

		void addActors();

		void onStart();

		void destroyActors();

	private:

		ContentLoader contentLoader;

		GameConfigConstants gameConfigConstants;

		RenderingConfigConstants renderingConfigConstants;

		GameConfigRegistry gameConfigRegistry;
		
		RenderConfigRegistry renderConfigRegistry;

		DataBank gameConfigBank;
		
		DataBank renderConfigBank;
		
		WindowManager windowManager;

		TextManager textManager;

		LuaManager luaManager;

		SceneManager sceneManager;

		ActorManager actorManager;

		bool running;
	};

}

#endif