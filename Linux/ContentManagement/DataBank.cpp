#include "DataBank.h"
#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include "rapidjson/include/rapidjson/document.h"

void VengefulGameEngine::DataBank::registerString(std::string key, std::string defaultValue)
{
	stringRegistry[key] = defaultValue;
}

void VengefulGameEngine::DataBank::registerFloat(std::string key, float defaultValue)
{
	floatRegistry[key] = defaultValue;
}

void VengefulGameEngine::DataBank::registerInt(std::string key, int defaultValue)
{
	intRegistry[key] = defaultValue;
}

void VengefulGameEngine::DataBank::loadBanks(std::string directory, std::string fileName, ContentLoader& contentLoader)
{
	if (!contentLoader.filePathExists(directory)) {
		std::cout << "error: " << directory << " missing";
		exit(0);
	}

	if (!contentLoader.fileExists(directory, fileName)) {
		std::cout << "error: " << fileName << " missing";
		exit(0);
	}

	rapidjson::Document document;
	contentLoader.loadRapidJSON(document, directory, fileName);

	if (document.HasParseError()) {
		std::cout << "error: document for " << fileName << " has parse error";
		exit(0);
	}

	for (auto& item : stringRegistry) {
		std::string key = item.first;
		if (document.HasMember(key.c_str())) {
			stringBank[key] = document[key.c_str()].GetString();
		}
		else {
			stringBank[key] = item.second;
		}
	}

	for (auto& item : floatRegistry) {
		std::string key = item.first;
		if (document.HasMember(key.c_str())) {
			floatBank[key] = document[key.c_str()].GetFloat();
		}
		else {
			floatBank[key] = item.second;
		}
	}

	for (auto& item : intRegistry) {
		std::string key = item.first;
		if (document.HasMember(key.c_str())) {
			intBank[key] = document[key.c_str()].GetInt();
		}
		else {
			intBank[key] = item.second;
		}
	}
}

void VengefulGameEngine::DataBank::print()
{
	for (auto& item : stringRegistry) {
		std::string key = item.first;
		std::cout << key << "\t\t" << stringBank[key] << std::endl;
	}

	for (auto& item : floatRegistry) {
		std::string key = item.first;
		std::cout << key << "\t\t" << floatBank[key] << std::endl;
	}

	for (auto& item : intRegistry) {
		std::string key = item.first;
		std::cout << key << "\t\t" << intBank[key] << std::endl;
	}

}
