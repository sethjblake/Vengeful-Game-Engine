#include "ContentLoader.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include <iostream>
#include <filesystem>
#include <cstdlib>
#include "EngineConfig/Macros.h"

using std::string;

bool VengefulGameEngine::ContentLoader::filePathExists(string path)
{
	return std::filesystem::exists(path);
}

bool VengefulGameEngine::ContentLoader::fileExists(string path, string fileName)
{
	return std::filesystem::exists(path + "/" + fileName);
}

void VengefulGameEngine::ContentLoader::assertFilePathExists(std::string filePath)
{
	if (!filePathExists(filePath)) {
		std::cout << "error: " << filePath << " missing";
		exit(0);
	}
}

void VengefulGameEngine::ContentLoader::loadRapidJSON(rapidjson::Document& out_document, string path, string fileName)
{
	FILE* file_pointer = nullptr;
	string filePath = path + "/" + fileName;

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
	out_document.ParseStream(stream);
	std::fclose(file_pointer);

	if (out_document.HasParseError()) {
		rapidjson::ParseErrorCode errorCode = out_document.GetParseError();
		std::cout << "error parsing json at [resources]" << std::endl;
		exit(0);
	}
}

void VengefulGameEngine::ContentLoader::ReadJsonFile(rapidjson::Document& out_document)
{
	std::string resourcesFolder = RESOURCES_FOLDER;

	if (!(std::filesystem::exists(resourcesFolder.c_str()))) {
		std::cout << "error: " << resourcesFolder << "/ missing";
		exit(0);
	}

	std::string filePath = resourcesFolder + "/game.config";

	if (!(std::filesystem::exists(filePath.c_str()))) {
		std::cout << "error: " << resourcesFolder << "/game.config missing";
		exit(0);
	}

	FILE* file_pointer = nullptr;
#ifdef _WIN32
	fopen_s(&file_pointer, filePath.c_str(), "rb");
#else
	file_pointer = fopen(filePath.c_str(), "rb");
#endif
	char buffer[65536];
	rapidjson::FileReadStream stream(file_pointer, buffer, sizeof(buffer));
	out_document.ParseStream(stream);
	std::fclose(file_pointer);

	if (out_document.HasParseError()) {
		rapidjson::ParseErrorCode errorCode = out_document.GetParseError();
		std::cout << "error parsing json at [resources]" << std::endl;
		exit(0);
	}
}

string VengefulGameEngine::ContentLoader::getString(rapidjson::Document& document, string key)
{
	if (!document.HasMember(key.c_str()) || !document[key.c_str()].IsString()) {
		exit(0);
	}
	return document[key.c_str()].GetString();
}

string VengefulGameEngine::ContentLoader::getStringOrDefault(rapidjson::Document& document, string key, string defaultReturnValue) {
	if (!document.HasMember(key.c_str())) {
		return defaultReturnValue;
	}
	return document[key.c_str()].GetString();
}


int VengefulGameEngine::ContentLoader::getInt(rapidjson::Document& document, string key)
{
	if (!document.HasMember(key.c_str()) || !document[key.c_str()].IsInt()) {
		exit(0);
	}
	return document[key.c_str()].GetInt();
}

int VengefulGameEngine::ContentLoader::getIntOrDefault(rapidjson::Document& document, std::string key, int defaultReturnValue)
{
	if (!document.HasMember(key.c_str())) {
		return defaultReturnValue;
	}
	return document[key.c_str()].GetInt();
}

bool VengefulGameEngine::ContentLoader::isConfigured(rapidjson::Document& document, string key)
{
	return document.HasMember(key.c_str());
}

