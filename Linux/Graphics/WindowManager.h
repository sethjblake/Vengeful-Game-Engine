#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include <string>
#include <queue>
#include <vector>
#include <map>
#include "Sprite.h"
#include "SDL2/Helper.h"
#include "SDL2/include/SDL.h"
#include "glm/glm/glm.hpp"
#include "InputSystem/InputSystem.h"
#include "TextManager.h"
#include "ImagesManager.h"

using LAYER_NUMBER = int;


namespace VengefulGameEngine {
	
	class WindowManager {
	public:

		void init(std::string game_title, int posX, int posY, int colliderWidth, int colliderHeight, Uint32 flags, uint8_t clearR, uint8_t clearG, uint8_t clearB, float zoom, float offsetX, float offsetY, float easeFactor);
		
		void preRender(VengefulGameEngine::ContentLoader& contentLoader);

		void render(VengefulGameEngine::ContentLoader& contentLoader);

		void postRender();

		void renderImages(SDL_Renderer* renderer);

		void renderUI(SDL_Renderer* renderer);

		void renderText(SDL_Renderer* renderer);

		void renderPixels(SDL_Renderer* renderer);

		SDL_Renderer* getRenderer();
	
	private:

		SDL_Window* window;
		
		SDL_Renderer* renderer;

		TextManager textManager;

		ImagesManager imagesManager;

		SDL_Event event;
		
		SDL_Color clearColor;
		
		float zoomFactor;
		
		float zoomFactorInverse;
		
		std::queue<Sprite*> backgroundRenderQueue;
		
		std::queue<Sprite*> foregroundRenderQueue;
		
		float camOffsetX;
		
		float camOffsetY;
		
		glm::ivec2 windowDimensions;
		
		glm::ivec2 halfWindowDimensions;
		
		glm::vec2 cameraRootPosition = { 0 , 0 };
		
		glm::vec2 cameraFocus = { 0, 0 };
		
		glm::vec2 targetCameraFocus = { 0, 0 };
		
		float camEaseFactor;

	};
}

#endif