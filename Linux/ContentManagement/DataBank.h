#ifndef DATA_BANK_H
#define DATA_BANK_H

#include <string>
#include <unordered_set>
#include <unordered_map>
#include "ContentLoader.h"
#include "Actors/Actor.h"

namespace VengefulGameEngine {

	class DataBank {
	public:
		
		void registerString(std::string key, std::string defaultValue = "");
		
		void registerFloat(std::string key, float defaultValue = 0.0f);
		
		void registerInt(std::string key, int defaultValue = 0);

		void loadBanks(std::string directory, std::string fileName, ContentLoader& contentLoader);

		void print();

		std::unordered_map<std::string, std::string> stringRegistry;
		
		std::unordered_map<std::string, float> floatRegistry;
		
		std::unordered_map<std::string, int> intRegistry;

		std::unordered_map<std::string, std::string> stringBank;
		
		std::unordered_map<std::string, float> floatBank;
		
		std::unordered_map<std::string, int> intBank;
	};
}

#endif