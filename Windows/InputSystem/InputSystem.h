#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

#include "../../ThirdParty/SDL2/include/SDL.h"
#include <string>
#include "../../ThirdParty/SDL2/Helper.h"
#include <queue>
#include <map>
#include "../ThirdParty/glm/glm/glm.hpp"

namespace VengefulGameEngine {

	struct EventTrigger {
		bool thisFrame = false;
		bool lastFrame = false;
	};

	class InputSystem {
	public:
		void readInput(bool& running);
		void saveKeysPressedThisFrame();
		void setKeyControlNames(std::vector<std::pair<std::string, SDL_Scancode>>& nameKeyPairs);
		
		void initMouseButtons();

		bool keyBecameDownThisFrame(std::string keyName);
		bool keyBecameUpThisFrame(std::string keyName);
		bool keyDown(std::string keyName);

		bool mouseButtonBecameDownThisFrame(int buttonNumber);
		bool mouseButtonBecameUpThisFrame(int buttonNumber);
		bool mouseButtonDown(int buttonNumber);

		glm::vec2 getMousePosition();
		void resetEventTriggerMaps();
		float getMouseScrollDelta();
	private:
		std::unordered_map<std::string, SDL_Scancode> keyNameMap;
		std::unordered_map<SDL_Scancode, EventTrigger> keyTriggerMap;
		std::unordered_map<Uint8, EventTrigger> mouseTriggerMap;
		glm::vec2 mousePosition = {0,0};
		float mouseScrollDelta = 0.0f;
	};
}

#endif