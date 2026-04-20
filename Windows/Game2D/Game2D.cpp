#include "Game2D.h"
#include <iostream>
#include "../../ThirdParty/Lua/lua.hpp"
#include "../Actors/GlobalActorsMap.h"
#include "../LuaManager/LuaInput.h"
#include "../AudioSystem/GlobalAudioManager.h"
#include "../ContentManagement/GlobalSceneData.h"
#include "Physics/GlobalPhysicsSystem.h"
#include "EngineConfig/Macros.h"
#include "LuaManager/LuaActorHelper.h"
#include "Events/GlobalEventSystem.h"
#include "Particles/GlobalParticleSystemsManager.h"
#include "Progress/GlobalProgressManager.h"

void VengefulGameEngine::Game2D::initBeforeFirstRender()
{
	running = true;
	initLua();
	LuaCpp::initGlobalInputSystem();
	checkIfResourcesExists();
	loadGameConfig();
	loadRenderConfig();
	initWindow();
	initAudio();
	initPhysics();
	globalProgressManager.init();
	loadNewScene(gameConfigBank.stringBank[gameConfigConstants.initial_scene]);
}

bool VengefulGameEngine::Game2D::isRunning()
{
	return running;
}

void VengefulGameEngine::Game2D::readInput()
{
	if (globalSceneData.newSceneName.has_value()) {
		loadNewScene(globalSceneData.newSceneName.value());
	}
	LuaCpp::readInput(running);
}

void VengefulGameEngine::Game2D::addActors()
{
	if (newActorsQueue.empty()) return;

	for (Actor*& actor : newActorsQueue) {

		int id = actor->uniqueActorID;

		if (globalActorsMap.find(id) != globalActorsMap.end()) {
			std::cout << "error: repeat actor id " << id;
		}

		globalActorsMap[id] = actor;

		for (auto& item : actor->components) {
			std::string key = item.first;
			std::shared_ptr<Component>& componentPtr = item.second;

			luabridge::LuaRef& luaRef = *componentPtr->luaRef;

			luabridge::LuaRef startFunction = luaRef["OnStart"];

			if (!startFunction.isNil() && startFunction.isFunction()) {
				actorManager.onStartActorKeySets[id].insert(componentPtr->key);
			}

			luabridge::LuaRef updateFunction = luaRef["OnUpdate"];

			if (!updateFunction.isNil() && updateFunction.isFunction()) {
				actorManager.onUpdateActorKeySets[id].insert(componentPtr->key);
			}

			luabridge::LuaRef lateUpdateFunction = luaRef["OnLateUpdate"];

			if (!lateUpdateFunction.isNil() && lateUpdateFunction.isFunction()) {
				actorManager.lateUpdateActorKeySets[id].insert(componentPtr->key);
			}

		}
	}

	newActorsQueue.clear();
}

void VengefulGameEngine::Game2D::addComponents()
{
	if (newComponentsQueue.empty()) return;

	for (std::pair<int, std::shared_ptr<Component>>& item : newComponentsQueue) {
		int id = item.first;
		std::shared_ptr<Component>& componentPtr = item.second;
		globalActorsMap[id]->components[componentPtr->key] = componentPtr;

		luabridge::LuaRef& luaRef = *componentPtr->luaRef;

		luabridge::LuaRef startFunction = luaRef["OnStart"];

		if (!startFunction.isNil() && startFunction.isFunction()) {
			actorManager.onStartActorKeySets[id].insert(componentPtr->key);
		}

		luabridge::LuaRef updateFunction = luaRef["OnUpdate"];

		if (!updateFunction.isNil() && updateFunction.isFunction()) {
			actorManager.onUpdateActorKeySets[id].insert(componentPtr->key);
		}

		luabridge::LuaRef lateUpdateFunction = luaRef["OnLateUpdate"];

		if (!lateUpdateFunction.isNil() && lateUpdateFunction.isFunction()) {
			actorManager.lateUpdateActorKeySets[id].insert(componentPtr->key);
		}

	}

	newComponentsQueue.clear();
}

void VengefulGameEngine::Game2D::update()
{
	addActors();
	addComponents();
	globalPhysicsSystem.initRigidbodiesOnUpdate();
	onStart();

	std::string functionName = "OnUpdate";

	for (auto& item : actorManager.onUpdateActorKeySets) {
		int id = item.first;
		Actor* actor = globalActorsMap[id];
		std::set<std::string>& keysSet = item.second;

		for (const std::string& key : keysSet) {
			std::shared_ptr<Component> component = actor->components[key];
			safeCallLuaFunction(actor->name, functionName, component->luaRef, true);
		}
	}

	globalParticleSystemsManager.CallUpdate();
}

void VengefulGameEngine::Game2D::render()
{
	windowManager.render(contentLoader);
}

void VengefulGameEngine::Game2D::lateUpdate()
{
	std::string functionName = "OnLateUpdate";
	for (auto& item : actorManager.lateUpdateActorKeySets) {
		int id = item.first;
		Actor* actor = globalActorsMap[id];
		std::set<std::string>& keysSet = item.second;

		for (const std::string& key : keysSet) {
			std::shared_ptr<Component> component = actor->components[key];
			safeCallLuaFunction(actor->name, functionName, component->luaRef, true);
		}
	}

	globalEventSystem.updateSubscriptions();

	globalPhysicsSystem.updateAfterActors();
	globalPhysicsSystem.clearHitResults();

	LuaCpp::saveKeysPressedThisFrame();

	removeComponents();
	destroyActors();
}

void VengefulGameEngine::Game2D::afterGameEnd()
{
	luaManager.closeLua();
}

void VengefulGameEngine::Game2D::checkIfResourcesExists()
{
	std::string resourcesFolder = RESOURCES_FOLDER;
	contentLoader.assertFilePathExists(resourcesFolder);
}

void VengefulGameEngine::Game2D::loadGameConfig()
{
	gameConfigRegistry.loadRegistry(gameConfigBank);
	std::string resourcesFolder = RESOURCES_FOLDER;
	gameConfigBank.loadBanks(resourcesFolder, "game.config", contentLoader);
}

void VengefulGameEngine::Game2D::loadRenderConfig()
{
	renderConfigRegistry.loadRegistry(renderConfigBank);
	std::string resourcesFolder = RESOURCES_FOLDER;
	renderConfigBank.loadBanks(resourcesFolder, "rendering.config", contentLoader);
}

void VengefulGameEngine::Game2D::initWindow()
{
	windowManager.init(
		gameConfigBank.stringBank[gameConfigConstants.game_title],
		100,
		100,
		renderConfigBank.intBank[renderingConfigConstants.x_resolution],
		renderConfigBank.intBank[renderingConfigConstants.y_resolution],
		0,
		static_cast<uint8_t>(renderConfigBank.intBank[renderingConfigConstants.clear_color_r]),
		static_cast<uint8_t>(renderConfigBank.intBank[renderingConfigConstants.clear_color_g]),
		static_cast<uint8_t>(renderConfigBank.intBank[renderingConfigConstants.clear_color_b]),
		renderConfigBank.floatBank[renderingConfigConstants.zoom_factor],
		renderConfigBank.floatBank[renderingConfigConstants.cam_offset_x],
		renderConfigBank.floatBank[renderingConfigConstants.cam_offset_y],
		renderConfigBank.floatBank[renderingConfigConstants.cam_ease_factor]
	);
}

void VengefulGameEngine::Game2D::initAudio()
{
	globalAudioManager.init();
}

void VengefulGameEngine::Game2D::initLua()
{
	luaManager.init();
}

void VengefulGameEngine::Game2D::initPhysics()
{
	float initialGravityX = 0.0f;
	float initialGravityY = 9.8f;
	float stepA = 1.0f / 60.0f;
	int32_t stepB = 8;
	int32_t stepC = 3;
	globalPhysicsSystem.init(
		initialGravityX,
		initialGravityY,
		stepA,
		stepB,
		stepC);
}

void VengefulGameEngine::Game2D::loadNewScene(std::string sceneName)
{
	globalSceneData.currentScene = sceneName;
	globalSceneData.newSceneName.reset();
	sceneManager.clearScene(actorManager);
	sceneManager.loadNewScene(sceneName, actorManager, contentLoader, luaManager);
}

void VengefulGameEngine::Game2D::removeComponents()
{
	if (removeComponentQueue.empty()) return;

	std::string functionName = "OnDestroy";

	for (std::pair<int, std::shared_ptr<Component>>& item : removeComponentQueue) {
		int id = item.first;
		std::shared_ptr<Component>& component = item.second;

		if (globalActorsMap.find(id) != globalActorsMap.end()) {
			Actor* actor = globalActorsMap[id];
			safeCallLuaFunction(actor->name, functionName, component->luaRef, false);
			actor->components.erase(component->key);
		}

		for (Actor* actor : newActorsQueue) {
			if (actor->uniqueActorID == id && actor->components.find(component->key) != actor->components.end()) {
				actor->components.erase(component->key);
				break;
			}
		}
		
		if (actorManager.onStartActorKeySets.find(id) != actorManager.onStartActorKeySets.end()) {
			std::set<std::string>& componentKeys = actorManager.onStartActorKeySets[id];
			if (componentKeys.find(component->key) != componentKeys.end()) {
				componentKeys.erase(component->key);
			}
		}

		if (actorManager.onUpdateActorKeySets.find(id) != actorManager.onUpdateActorKeySets.end()) {
			std::set<std::string>& componentKeys = actorManager.onUpdateActorKeySets[id];
			if (componentKeys.find(component->key) != componentKeys.end()) {
				componentKeys.erase(component->key);
			}
		}

		if (actorManager.lateUpdateActorKeySets.find(id) != actorManager.lateUpdateActorKeySets.end()) {
			std::set<std::string>& componentKeys = actorManager.lateUpdateActorKeySets[id];
			if (componentKeys.find(component->key) != componentKeys.end()) {
				componentKeys.erase(component->key);
			}
		}
	}
	removeComponentQueue.clear();
}

void VengefulGameEngine::Game2D::onStart()
{
	globalParticleSystemsManager.CallOnStart();

	if (actorManager.onStartActorKeySets.empty()) return;

	std::map<int, std::set<std::string>>& onStartActorKeySets = actorManager.onStartActorKeySets;

	std::string functionName = "OnStart";

	for (auto& item : onStartActorKeySets) {
		int id = item.first;
		Actor* actor = globalActorsMap[id];
		std::set<std::string>& keysSet = item.second;

		for (const std::string& key : keysSet) {
			std::shared_ptr<Component> component = actor->components[key];
			safeCallLuaFunction(actor->name, functionName, component->luaRef, true);
		}
	}

	onStartActorKeySets.clear();
}

void VengefulGameEngine::Game2D::destroyActors()
{
	if (destroyActorsQueue.empty()) return;

	for (Actor*& actor : destroyActorsQueue) {

		actor->callLuaFunctionOnComponents("OnDestroy", false);

		if (globalActorsMap.find(actor->uniqueActorID) != globalActorsMap.end()) {
			globalActorsMap.erase(actor->uniqueActorID);
		}
		else {
			std::vector<Actor*>::iterator itr = newActorsQueue.begin();
			for (; itr != newActorsQueue.end(); itr++) {
				if ((*itr)->uniqueActorID == actor->uniqueActorID) {
					break;
				}
			}
			newActorsQueue.erase(itr);
		}

		delete actor;
		actor = nullptr;
	}

	destroyActorsQueue.clear();
}
