#ifndef GAME_CONFIG_REGISTRY_H
#define GAME_CONFIG_REGISTRY_H

#include "DataBank.h"
#include "Constants/GameConfigConstants.h"

namespace VengefulGameEngine {

	class GameConfigRegistry {
	public:
		void loadRegistry(DataBank& gameConfigBank);
		GameConfigConstants gameConfigConstants;
	};
}

#endif