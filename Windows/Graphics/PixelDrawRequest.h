#ifndef PIXEL_DRAW_REQUEST_H
#define PIXEL_DRAW_REQUEST_H

#include "../../ThirdParty/glm/glm/glm.hpp"
#include "../../ThirdParty/SDL2/include/SDL.h"

namespace VengefulGameEngine {

	struct PixelDrawRequest {

		glm::ivec2 position;

		SDL_Color color;

	};

}

#endif