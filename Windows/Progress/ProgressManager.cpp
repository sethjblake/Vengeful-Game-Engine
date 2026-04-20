#include "ProgressManager.h"

#include "EngineConfig/Macros.h"
#include "Lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"
#include "LuaManager/LuaState.h"
#include "../../ThirdParty/rapidjson/include/rapidjson/rapidjson.h"
#include "../../ThirdParty/rapidjson/include/rapidjson/document.h"
#include "../../ThirdParty/rapidjson/include/rapidjson/stringbuffer.h"
#include "../../ThirdParty/rapidjson/include/rapidjson/writer.h"
#include "../../ThirdParty/rapidjson/include/rapidjson/filewritestream.h"
#include "../../ThirdParty/rapidjson/include/rapidjson/filereadstream.h"
#include <string>
#include <filesystem>
#include <cstdio>
#include <iostream>
#include <memory>

void VengefulGameEngine::ProgressManager::init()
{
	Load();
}

void VengefulGameEngine::ProgressManager::Load()
{

	ensureTableExists(intTable);
	ensureTableExists(floatTable);
	ensureTableExists(stringTable);
	ensureTableExists(boolTable);

	rapidjson::Document intDocument;
	rapidjson::Document floatDocument;
	rapidjson::Document stringDocument;
	rapidjson::Document boolDocument;

	std::string resourcesFolder = RESOURCES_FOLDER;
	std::string filePath = resourcesFolder + "/saved_data/";

	std::string intFilePath = filePath + "ints.json";
	std::string floatFilePath = filePath + "floats.json";
	std::string stringFilePath = filePath + "strings.json";
	std::string boolFilePath = filePath + "bools.json";

	ensureFileExists(intFilePath);
	ensureFileExists(floatFilePath);
	ensureFileExists(stringFilePath);
	ensureFileExists(boolFilePath);

	loadDocument(intFilePath, intDocument);
	loadDocument(floatFilePath, floatDocument);
	loadDocument(stringFilePath, stringDocument);
	loadDocument(boolFilePath, boolDocument);

	for (auto& member : intDocument.GetObject()) {
		std::string key = member.name.GetString();
		const auto& value = member.value;
		if (value.IsInt()) {
			int temp = value.GetInt();
			(*intTable)[key.c_str()] = temp;
		}
	}

	for (auto& member : floatDocument.GetObject()) {
		std::string key = member.name.GetString();
		const auto& value = member.value;
		if (value.IsFloat()) {
			float temp = value.GetFloat();
			(*floatTable)[key.c_str()] = temp;
		}
	}

	for (auto& member : stringDocument.GetObject()) {
		std::string key = member.name.GetString();
		const auto& value = member.value;
		if (value.IsString()) {
			std::string temp = value.GetString();
			(*stringTable)[key.c_str()] = temp;
		}
	}

	for (auto& member : boolDocument.GetObject()) {
		std::string key = member.name.GetString();
		const auto& value = member.value;
		if (value.IsBool()) {
			bool temp = value.GetBool();
			(*boolTable)[key.c_str()] = temp;
		}
	}
}

void VengefulGameEngine::ProgressManager::Save()
{
	ensureTableExists(intTable);
	ensureTableExists(floatTable);
	ensureTableExists(stringTable);
	ensureTableExists(boolTable);

	rapidjson::Document intDocument;
	rapidjson::Document floatDocument;
	rapidjson::Document stringDocument;
	rapidjson::Document boolDocument;

	std::string resourcesFolder = RESOURCES_FOLDER;
	std::string filePath = resourcesFolder + "/saved_data/";

	std::string intFilePath = filePath + "ints.json";
	std::string floatFilePath = filePath + "floats.json";
	std::string stringFilePath = filePath + "strings.json";
	std::string boolFilePath = filePath + "bools.json";

	ensureFileExists(intFilePath);
	ensureFileExists(floatFilePath);
	ensureFileExists(stringFilePath);
	ensureFileExists(boolFilePath);

	loadDocument(intFilePath, intDocument);
	loadDocument(floatFilePath, floatDocument);
	loadDocument(stringFilePath, stringDocument);
	loadDocument(boolFilePath, boolDocument);

	for (luabridge::Iterator it(*intTable); !it.isNil(); ++it) {
		luabridge::LuaRef keyRef = it.key();
		luabridge::LuaRef value = it.value();

		std::string key;
		if (keyRef.isString()) {
			key = keyRef.cast<std::string>();
		}
		else {
			continue;
		}
		
		if (!value.isNumber()) continue;

		if (intDocument.HasMember(key.c_str())) {
			if (intDocument[key.c_str()].IsInt()) {
				intDocument[key.c_str()] = value.cast<int>();
			}
		}
		else {
			rapidjson::Value keyValue(key.c_str(), intDocument.GetAllocator());
				intDocument.AddMember(keyValue, value.cast<int>(), intDocument.GetAllocator());
		}
	}


	for (luabridge::Iterator it(*floatTable); !it.isNil(); ++it) {
		luabridge::LuaRef keyRef = it.key();
		luabridge::LuaRef value = it.value();
		std::string key;
		if (keyRef.isString()) {
			key = keyRef.cast<std::string>();
		}
		else {
			continue;
		}
		
		if (!value.isNumber()) continue;

		if (floatDocument.HasMember(key.c_str())) {
			if (floatDocument[key.c_str()].IsFloat()) {
				floatDocument[key.c_str()] = value.cast<float>();
			}
		}
		else {
			rapidjson::Value keyValue(key.c_str(), floatDocument.GetAllocator());
			floatDocument.AddMember(keyValue, value.cast<float>(), floatDocument.GetAllocator());
		}
	}

	for (luabridge::Iterator it(*stringTable); !it.isNil(); ++it) {
		luabridge::LuaRef keyRef = it.key();
		luabridge::LuaRef value = it.value();
		std::string key;
		if (keyRef.isString()) {
			key = keyRef.cast<std::string>();
		}
		else {
			continue;
		}

		if (!value.isString()) continue;


		if (stringDocument.HasMember(key.c_str())) {
			if (stringDocument[key.c_str()].IsBool()) {
				std::string valStr = value.cast<std::string>();
				stringDocument[key.c_str()].SetString(valStr.c_str(),
					static_cast<rapidjson::SizeType>(valStr.length()),
					stringDocument.GetAllocator());
			}
		}
		else {
			rapidjson::Value keyValue(key.c_str(), boolDocument.GetAllocator());
			rapidjson::Value str(value.cast<std::string>().c_str(), stringDocument.GetAllocator());
			stringDocument.AddMember(keyValue, str, stringDocument.GetAllocator());
		}
	}

	for (luabridge::Iterator it(*boolTable); !it.isNil(); ++it) {
		luabridge::LuaRef keyRef = it.key();
		luabridge::LuaRef value = it.value();
		std::string key;
		if (keyRef.isString()) {
			key = keyRef.cast<std::string>();
		}
		else {
			continue;
		}

		if (!value.isBool()) continue;

		if (boolDocument.HasMember(key.c_str())) {
			if (boolDocument[key.c_str()].IsBool()) {
				boolDocument[key.c_str()] = value.cast<bool>();
			}
		}
		else {
			rapidjson::Value keyValue(key.c_str(), boolDocument.GetAllocator());
			boolDocument.AddMember(keyValue, value.cast<bool>(), boolDocument.GetAllocator());
		}
	}

	saveDocument(intFilePath, intDocument);
	saveDocument(floatFilePath, floatDocument);
	saveDocument(stringFilePath, stringDocument);
	saveDocument(boolFilePath, boolDocument);
}

void VengefulGameEngine::ProgressManager::SetInt(std::string key, int value)
{
	ensureTableExists(intTable);
	(*intTable)[key.c_str()] = value;
}

luabridge::LuaRef VengefulGameEngine::ProgressManager::GetInt(std::string key)
{

	ensureTableExists(intTable);

	luabridge::LuaRef value = (*intTable)[key.c_str()];

	if (value.isNil()) {
		return luabridge::LuaRef(luaState);
	}

	luabridge::LuaRef table = luabridge::LuaRef::newTable(luaState);
	table[key.c_str()] = value.cast<int>();
	return table;
}

void VengefulGameEngine::ProgressManager::SetFloat(std::string key, float value)
{
	ensureTableExists(floatTable);
	(*floatTable)[key.c_str()] = value;
}

luabridge::LuaRef VengefulGameEngine::ProgressManager::GetFloat(std::string key)
{
	ensureTableExists(floatTable);
	return (*floatTable)[key.c_str()];
}

void VengefulGameEngine::ProgressManager::SetString(std::string key, std::string value)
{
	ensureTableExists(stringTable);
	(*stringTable)[key.c_str()] = value;
}

luabridge::LuaRef VengefulGameEngine::ProgressManager::GetString(std::string key)
{
	ensureTableExists(stringTable);
	return (*stringTable)[key.c_str()];
}

void VengefulGameEngine::ProgressManager::SetBool(std::string key, bool value)
{
	ensureTableExists(boolTable);
	(*boolTable)[key.c_str()] = value;
}

luabridge::LuaRef VengefulGameEngine::ProgressManager::GetBool(std::string key)
{
	ensureTableExists(stringTable);
	return (*stringTable)[key.c_str()];
}

void VengefulGameEngine::ProgressManager::ensureFileExists(std::string filePath)
{
	if (std::filesystem::exists(filePath)) {
		return;
	}

	rapidjson::Document blankDocument;
	blankDocument.SetObject();

	saveDocument(filePath, blankDocument);
}

void VengefulGameEngine::ProgressManager::ensureTableExists(std::shared_ptr<luabridge::LuaRef>& tablePointer)
{
	if (tablePointer == nullptr) tablePointer = std::make_shared<luabridge::LuaRef>(luabridge::LuaRef::newTable(luaState));
}

void VengefulGameEngine::ProgressManager::loadDocument(std::string filePath, rapidjson::Document& document)
{

	if (!std::filesystem::exists(filePath)) {
		return;
	}

	FILE* file_pointer = nullptr;

	if (!std::filesystem::exists(filePath)) {
		std::cout << "error: " + filePath + " missing";
		exit(0);
	}

#ifdef _WIN32
	fopen_s(&file_pointer, filePath.c_str(), "rb");
#else
	file_pointer = fopen(filePath.c_str(), "rb");
#endif
	char buffer[65536];
	rapidjson::FileReadStream stream(file_pointer, buffer, sizeof(buffer));
	document.ParseStream(stream);
	std::fclose(file_pointer);

	if (document.HasParseError()) {
		rapidjson::ParseErrorCode errorCode = document.GetParseError();
		std::cout << "error parsing json at [resources]" << std::endl;
		exit(0);
	}

	if (!document.IsObject()) {
		document.SetObject();
	}
}

void VengefulGameEngine::ProgressManager::saveDocument(std::string filePath, rapidjson::Document& document)
{
	FILE* fp;
	fopen_s(&fp, filePath.c_str(), "wb");
	char writeBuffer[65536];
	rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
	rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
	document.Accept(writer);
	fclose(fp);
}
