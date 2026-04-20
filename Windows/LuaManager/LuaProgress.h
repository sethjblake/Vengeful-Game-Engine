#ifndef LUA_PROGRESS_H
#define LUA_PROGRESS_H

#include "Progress/GlobalProgressManager.h"
#include <string>

namespace VengefulGameEngine {

	static inline void SetInt(std::string key, int i) {
		globalProgressManager.SetInt(key, i);
	}

	static inline void SetFloat(std::string key, float f) {
		globalProgressManager.SetFloat(key, f);
	}

	static inline void SetString(std::string key, std::string s) {
		globalProgressManager.SetString(key, s);
	}

	static inline void SetBool(std::string key, bool b) {
		globalProgressManager.SetFloat(key, b);
	}

	static inline luabridge::LuaRef GetInt(std::string key) {
		return globalProgressManager.GetInt(key);
	}

	static inline luabridge::LuaRef GetFloat(std::string key) {
		return globalProgressManager.GetFloat(key);
	}

	static inline luabridge::LuaRef GetString(std::string key) {
		return globalProgressManager.GetString(key);
	}

	static inline luabridge::LuaRef GetBool(std::string key) {
		return globalProgressManager.GetBool(key);
	}

	static inline void Save() {
		globalProgressManager.Save();
	}
}

#endif