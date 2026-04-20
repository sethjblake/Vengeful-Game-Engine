#ifndef PARTICLE_H
#define PARTICLE_H


#include "SDL2/include/SDL.h"
#include "SDL2/include/SDL_image.h"
#include "glm/glm/glm.hpp"
#include "Math/PolarCoordinates.h"
#include <string>

namespace VengefulGameEngine {
	struct Particle {

		std::string textureName = "VengefulGameEngineDefaultParticle";

		SDL_Texture* texture = nullptr;

		SDL_Color color = { 255, 255, 255, 255 };
		
		glm::vec2 position = { 0.0f, 0.0f };

		int width = 8;

		int height = 8;

		glm::vec2 pivotPoint = { 0.5f, 0.5f };

		glm::vec2 scale = { 1.0f, 1.0f };

		glm::vec2 initialScale = { 1.0f, 1.0f };

		float rotation = 0.0f;

		int sortingOrder = 9999;

		int currentDuration = 300;

		glm::vec2 velocity = { 0.0f, 0.0f };

		float anglularVelocity = 0.0f;

		int framesParticleHasBeenAlive = 0;

		int durationFrames = 300;

	};
}

#endif