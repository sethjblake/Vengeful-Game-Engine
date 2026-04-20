#include "LuaInput.h"
#include <string>
#include "../ThirdParty/Lua/Lua.hpp"
#include "../ThirdParty/LuaBridge/LuaBridge.h"
#include "../InputSystem/InputSystem.h"
#include "../ThirdParty/SDL2/include/SDL.h"
#include "../InputSystem/GlobalInputSystem.h"
#include "../InputSystem/KeyCodeToScancode.h"
#include "LuaState.h"
#include <vector>
#include "../ThirdParty/glm/glm/glm.hpp"

void LuaCpp::initGlobalInputSystem()
{
	std::vector<std::pair<std::string, SDL_Scancode>> keyNames;

	for (auto& item : VengefulGameEngine::__keycode_to_scancode) {
		const std::string name = item.first;
		const SDL_Scancode code = item.second;
		std::pair<std::string, SDL_Scancode> nameCodePair(name, code);
		keyNames.push_back(nameCodePair);
	}

	VengefulGameEngine::globalInputSystem.setKeyControlNames(keyNames);

	VengefulGameEngine::globalInputSystem.initMouseButtons();
}

void LuaCpp::readInput(bool& running)
{
	VengefulGameEngine::globalInputSystem.readInput(running);
}

void LuaCpp::saveKeysPressedThisFrame()
{
	VengefulGameEngine::globalInputSystem.saveKeysPressedThisFrame();
}

bool LuaCpp::getKey(std::string keyCode) {
	return VengefulGameEngine::globalInputSystem.keyDown(keyCode);
}

bool LuaCpp::getKeyDown(std::string keyCode) {
	return VengefulGameEngine::globalInputSystem.keyBecameDownThisFrame(keyCode);
}

bool LuaCpp::getKeyUp(std::string keyCode) {
	return VengefulGameEngine::globalInputSystem.keyBecameUpThisFrame(keyCode);
}

glm::vec2 LuaCpp::getMousePosition() {
	return VengefulGameEngine::globalInputSystem.getMousePosition();
}

bool LuaCpp::getMouseButton(int buttonNumber) {
	return VengefulGameEngine::globalInputSystem.mouseButtonDown(buttonNumber);
}

bool LuaCpp::getMouseButtonDown(int buttonNumber) {
	return VengefulGameEngine::globalInputSystem.mouseButtonBecameDownThisFrame(buttonNumber);
}

bool LuaCpp::getMouseButtonUp(int buttonNumber) {
	return VengefulGameEngine::globalInputSystem.mouseButtonBecameUpThisFrame(buttonNumber);
}

float LuaCpp::getMouseScrollDelta() {
	return VengefulGameEngine::globalInputSystem.getMouseScrollDelta();
}

void LuaCpp::hideCursor() {
	SDL_ShowCursor(SDL_DISABLE);
}

void LuaCpp::showCursor() {
	SDL_ShowCursor(SDL_ENABLE);
}