#ifndef PROGRESS_MANAGER_H
#define PROGRESS_MANAGER_H

#include "Lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"
#include "../../ThirdParty/rapidjson/include/rapidjson/rapidjson.h"
#include "../../ThirdParty/rapidjson/include/rapidjson/document.h"
#include <string>
#include <memory>

namespace VengefulGameEngine {
	class ProgressManager {
	public:

		void init();

		void Load();

		void Save();

		void SetInt(std::string key, int value);

		luabridge::LuaRef GetInt(std::string key);

		void SetFloat(std::string key, float value);

		luabridge::LuaRef GetFloat(std::string key);

		void SetString(std::string key, std::string value);

		luabridge::LuaRef GetString(std::string key);

		void SetBool(std::string key, bool value);

		luabridge::LuaRef GetBool(std::string key);

	private:

		void ensureFileExists(std::string filePath);

		void ensureTableExists(std::shared_ptr<luabridge::LuaRef>& tablePointer);

		void loadDocument(std::string filePath, rapidjson::Document& document);

		void saveDocument(std::string filePath, rapidjson::Document& document);

		std::shared_ptr<luabridge::LuaRef> intTable = nullptr;

		std::shared_ptr<luabridge::LuaRef> floatTable = nullptr;
		
		std::shared_ptr<luabridge::LuaRef> stringTable = nullptr;
		
		std::shared_ptr<luabridge::LuaRef> boolTable = nullptr;

	};
}

#endif