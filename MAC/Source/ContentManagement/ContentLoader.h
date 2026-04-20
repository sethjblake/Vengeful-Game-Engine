#ifndef CONTENT_LOADER_H
#define CONTENT_LOADER_H

#include "rapidjson/document.h"
#include <string>

namespace VengefulGameEngine {
	class ContentLoader {
	public:
		
		void assertFilePathExists(std::string filePath);

		void loadRapidJSON(rapidjson::Document& out_document, std::string path, std::string fileName);

		void ReadJsonFile(rapidjson::Document& out_document);
		
		std::string getString(rapidjson::Document& document, std::string key);
		
		std::string getStringOrDefault(rapidjson::Document& document, std::string key, std::string defaultReturnValue);

		int getInt(rapidjson::Document& document, std::string key);
		
		int getIntOrDefault(rapidjson::Document& document, std::string key, int defaultReturnValue);

		bool filePathExists(std::string path);
		
		bool fileExists(std::string path, std::string fileName);
		
		bool isConfigured(rapidjson::Document& document, std::string key);
	};
}

#endif // CONTENT_LOADER_H
