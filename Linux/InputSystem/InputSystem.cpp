#include "InputSystem.h"
#include "SDL2/Helper.h"

void VengefulGameEngine::InputSystem::setKeyControlNames(std::vector<std::pair<std::string, SDL_Scancode>>& nameKeyPairs)
{
	for (std::pair<std::string, SDL_Scancode>& nameKeyPair : nameKeyPairs) {
		keyNameMap[nameKeyPair.first] = nameKeyPair.second;
		keyTriggerMap[nameKeyPair.second].thisFrame = false;
		keyTriggerMap[nameKeyPair.second].lastFrame = false;
	}
}

void VengefulGameEngine::InputSystem::initMouseButtons()
{
	EventTrigger defaultEventTrigger = { false, false };
	mouseTriggerMap[SDL_BUTTON_LEFT].thisFrame = false;
	mouseTriggerMap[SDL_BUTTON_LEFT].lastFrame = false;
	mouseTriggerMap[SDL_BUTTON_MIDDLE].thisFrame = false;
	mouseTriggerMap[SDL_BUTTON_MIDDLE].lastFrame = false;
	mouseTriggerMap[SDL_BUTTON_RIGHT].thisFrame = false;
	mouseTriggerMap[SDL_BUTTON_RIGHT].lastFrame = false;
}

void VengefulGameEngine::InputSystem::readInput(bool& running)
{

	resetEventTriggerMaps();

	SDL_Event event;

	while (Helper::SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			running = false;
			return;
		}

		if (event.type == SDL_KEYDOWN) {
			keyTriggerMap[event.key.keysym.scancode].thisFrame = true;
			keyTriggerMap[event.key.keysym.scancode].lastFrame = false;
		}

		if (event.type == SDL_KEYUP) {
			keyTriggerMap[event.key.keysym.scancode].thisFrame = false;
			keyTriggerMap[event.key.keysym.scancode].lastFrame = true;
		}

		if (event.type == SDL_MOUSEBUTTONDOWN) {
			Uint8 clickID = event.button.button;
			if (mouseTriggerMap.find(clickID) != mouseTriggerMap.end()) {
				mouseTriggerMap[clickID].thisFrame = true;
				mouseTriggerMap[clickID].lastFrame = false;
			}
		}

		if (event.type == SDL_MOUSEBUTTONUP) {
			Uint8 clickID = event.button.button;
			if (mouseTriggerMap.find(clickID) != mouseTriggerMap.end()) {
				mouseTriggerMap[clickID].thisFrame = false;
				mouseTriggerMap[clickID].lastFrame = true;
			}
		}

		if (event.type == SDL_MOUSEMOTION) {
			mousePosition.x = event.motion.x;
			mousePosition.y = event.motion.y;
		}

		if (event.type == SDL_MOUSEWHEEL) {
			mouseScrollDelta = event.wheel.preciseY;
		}
	}
}

void VengefulGameEngine::InputSystem::saveKeysPressedThisFrame()
{
	for (const std::pair<std::string, SDL_Scancode>& keyName : keyNameMap) {
		SDL_Scancode code = keyName.second;
		EventTrigger& eventTrigger = keyTriggerMap[code];
		eventTrigger.lastFrame = eventTrigger.thisFrame;
	}

	mouseTriggerMap[SDL_BUTTON_LEFT].lastFrame = mouseTriggerMap[SDL_BUTTON_LEFT].thisFrame;
	mouseTriggerMap[SDL_BUTTON_MIDDLE].lastFrame = mouseTriggerMap[SDL_BUTTON_MIDDLE].thisFrame;
	mouseTriggerMap[SDL_BUTTON_RIGHT].lastFrame = mouseTriggerMap[SDL_BUTTON_RIGHT].thisFrame;
}

bool VengefulGameEngine::InputSystem::keyBecameDownThisFrame(std::string keyName)
{
	if (keyNameMap.find(keyName) == keyNameMap.end()) {
		return false;
	}

	SDL_Scancode code = keyNameMap[keyName];
	return keyTriggerMap[code].thisFrame && !keyTriggerMap[code].lastFrame;
}

bool VengefulGameEngine::InputSystem::keyBecameUpThisFrame(std::string keyName)
{
	if (keyNameMap.find(keyName) == keyNameMap.end()) {
		return false;
	}

	SDL_Scancode code = keyNameMap[keyName];
	return !keyTriggerMap[code].thisFrame && keyTriggerMap[code].lastFrame;
}

bool VengefulGameEngine::InputSystem::keyDown(std::string keyName)
{
	if (keyNameMap.find(keyName) == keyNameMap.end()) {
		return false;
	}

	SDL_Scancode code = keyNameMap[keyName];
	return keyTriggerMap[code].thisFrame;
}

bool VengefulGameEngine::InputSystem::mouseButtonBecameDownThisFrame(int buttonNumber)
{
	Uint8 key = static_cast<Uint8>(buttonNumber);

	if (mouseTriggerMap.find(key) == mouseTriggerMap.end()) {
		return false;
	}

	EventTrigger& eventTrigger = mouseTriggerMap[key];

	return eventTrigger.thisFrame && !eventTrigger.lastFrame;
}

bool VengefulGameEngine::InputSystem::mouseButtonBecameUpThisFrame(int buttonNumber)
{
	Uint8 key = static_cast<Uint8>(buttonNumber);

	if (mouseTriggerMap.find(key) == mouseTriggerMap.end()) {
		return false;
	}

	EventTrigger& eventTrigger = mouseTriggerMap[key];

	return !eventTrigger.thisFrame && eventTrigger.lastFrame;
}

bool VengefulGameEngine::InputSystem::mouseButtonDown(int buttonNumber)
{
	Uint8 key = static_cast<Uint8>(buttonNumber);

	if (mouseTriggerMap.find(key) == mouseTriggerMap.end()) {
		return false;
	}

	EventTrigger& eventTrigger = mouseTriggerMap[key];

	return eventTrigger.thisFrame;
}

glm::vec2 VengefulGameEngine::InputSystem::getMousePosition() {
	return mousePosition;
}

void VengefulGameEngine::InputSystem::resetEventTriggerMaps()
{
	mouseScrollDelta = 0.0f;
}

float VengefulGameEngine::InputSystem::getMouseScrollDelta()
{
	return mouseScrollDelta;
}
