#include "GameConfigRegistry.h"

void VengefulGameEngine::GameConfigRegistry::loadRegistry(DataBank& gameConfigBank)
{
	gameConfigBank.registerString(gameConfigConstants.game_title);
	gameConfigBank.registerString(gameConfigConstants.font);
	gameConfigBank.registerString(gameConfigConstants.initial_scene);
}
