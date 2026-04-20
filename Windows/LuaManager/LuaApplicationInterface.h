#ifndef LUA_APPLICATION_INTERFACE_H
#define LUA_APPLICATION_INTERFACE_H

#include "../../ThirdParty/SDL2/Helper.h"
#include <thread>
#include <string>
#include <cstdlib>

namespace LuaApplicationInterface {
	
	void Quit() {
		exit(0);
	}

	int GetFrameNumber() {
		return Helper::GetFrameNumber();
	}

	void Sleep(int miliseconds) {
		std::this_thread::sleep_for(std::chrono::milliseconds(miliseconds));
	}

	void Open(std::string url) {
#ifdef _WIN32
		std::string systemMessage = "start " + url;
		std::system(systemMessage.c_str());
#endif // _WIN32
	}
}

#endif