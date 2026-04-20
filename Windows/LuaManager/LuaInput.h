#ifndef LUA_INPUT_H
#define LUA_INPUT_H

#include <string>
#include "../ThirdParty/Lua/Lua.hpp"
#include "../ThirdParty/LuaBridge/LuaBridge.h"
#include "../InputSystem/InputSystem.h"
#include "../ThirdParty/glm/glm/glm.hpp"

namespace LuaCpp {

	void initGlobalInputSystem();
	void readInput(bool& running);
	void saveKeysPressedThisFrame();

	bool getKey(std::string keyCode);
	bool getKeyDown(std::string keyCode);
	bool getKeyUp(std::string keyCode);
	glm::vec2 getMousePosition();
	bool getMouseButton(int buttonNumber);
	bool getMouseButtonDown(int buttonNumber);
	bool getMouseButtonUp(int buttonNumber);
	float getMouseScrollDelta();
	void hideCursor();
	void showCursor();
}

#endif