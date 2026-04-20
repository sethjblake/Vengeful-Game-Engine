#ifndef IMAGE_DRAW_REQUEST_H
#define IMAGE_DRAW_REQUEST_H

#include <string>
#include "glm/glm/glm.hpp"
#include "SDL2/include/SDL.h"

namespace VengefulGameEngine {
	struct ImageDrawRequest {
		std::string imageName;
		glm::vec2 position = { 0.0f, 0.0f };
		glm::vec2 scale = { 0.0f, 0.0f };
		glm::vec2 pivot = { 0.0f, 0.0f };
		int rotationDegrees = 0;
		SDL_Color color = { 255, 255, 255, 255 };
		int sortingOrder = 0;
	};
}

#endif