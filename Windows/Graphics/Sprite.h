#ifndef SPRITE_H
#define SPRITE_H

#include <string>
#include <queue>
#include "../../ThirdParty/SDL2/include/SDL_image.h"
#include "../../ThirdParty/SDL2/Helper.h"

namespace VengefulGameEngine {

	class Sprite {

	public:
		
		SDL_Texture* texture = nullptr;
		
		SDL_FRect* sourceRect = nullptr;
		
		SDL_FRect* destinationRect = nullptr;
		
		float colliderWidth;
		
		float colliderHeight;
		
		float rotationDegrees;
		
		SDL_FPoint* center = nullptr;
		
		SDL_RendererFlip flip = SDL_FLIP_NONE;

		SDL_Color color;

	};

}

#endif