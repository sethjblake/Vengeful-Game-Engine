#include "WindowManager.h"
#include "../../ThirdParty/SDL2/include/SDL.h"
#include "../../ThirdParty/SDL2/Helper.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include "Sprite.h"
#include "../../ThirdParty/glm/glm/glm.hpp"
#include "GlobalTextRequests.h"
#include "ImageDrawRequest.h"
#include "GlobalImageDrawRequests.h"
#include "PixelDrawRequest.h"
#include "GlobalPixelDrawRequests.h"
#include "GlobalCamera.h"

//#define RENDER_LOG

void VengefulGameEngine::WindowManager::init(std::string game_title, int posX, int posY, int colliderWidth, int colliderHeight, Uint32 flags, uint8_t clearR, uint8_t clearG, uint8_t clearB, float zoom, float offsetX, float offsetY, float easeFactor)
{
	window = Helper::SDL_CreateWindow(game_title.c_str(), posX, posY, colliderWidth, colliderHeight, flags);
	renderer = Helper::SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

	globalCamera.zoomFactor = zoom;
	globalCamera.zoomFactorInverse = 1 / zoom;
	globalCamera.position.x = offsetX;
	globalCamera.position.y = offsetY;

	clearColor.r = clearR;
	clearColor.g = clearG;
	clearColor.b = clearB;
	zoomFactorInverse = 1 / zoom;
	SDL_RenderSetScale(renderer, zoom, zoom);
	windowDimensions.x = colliderWidth;
	windowDimensions.y = colliderHeight;
	halfWindowDimensions.x = colliderWidth / 2;
	halfWindowDimensions.y = colliderHeight / 2;
	camOffsetX = offsetX;
	camOffsetY = offsetY;
	camEaseFactor = easeFactor;
	textManager.init();
}

void VengefulGameEngine::WindowManager::preRender(VengefulGameEngine::ContentLoader& contentLoader)
{
	if (newZoomFlag) {
		SDL_RenderSetScale(renderer, globalCamera.zoomFactor, globalCamera.zoomFactor);
		newZoomFlag = false;
	}

	imagesManager.generateImages(renderer, contentLoader);
	imagesManager.generateUI(renderer, contentLoader);
	textManager.generateTextures(renderer, contentLoader);
}

void VengefulGameEngine::WindowManager::render(VengefulGameEngine::ContentLoader& contentLoader)
{
	preRender(contentLoader);

	SDL_SetRenderDrawColor(renderer, clearColor.r, clearColor.g, clearColor.b, 255);
	SDL_RenderClear(renderer);

	renderImages(renderer);

	renderUI(renderer);

	renderText(renderer);

	renderPixels(renderer);

	Helper::SDL_RenderPresent(renderer);

	postRender();

}

void VengefulGameEngine::WindowManager::postRender()
{
	imagesManager.clearImages();
	imagesManager.clearUI();
	textManager.clearTextures();
}

void VengefulGameEngine::WindowManager::renderImages(SDL_Renderer* renderer)
{
	for (Sprite*& sprite : imagesManager.imageSprites) {

		SDL_FRect* destinationRect = sprite->destinationRect;

		destinationRect->x += halfWindowDimensions.x * globalCamera.zoomFactorInverse;
		destinationRect->y += halfWindowDimensions.y * globalCamera.zoomFactorInverse;

		destinationRect->x -= globalCamera.position.x * 100 * globalCamera.zoomFactorInverse;
		destinationRect->y -= globalCamera.position.y * 100 * globalCamera.zoomFactorInverse;

		SDL_SetTextureColorMod(sprite->texture,
			sprite->color.r, sprite->color.g, sprite->color.b);
		SDL_SetTextureAlphaMod(sprite->texture,
			sprite->color.a);
		SDL_SetTextureBlendMode(sprite->texture, SDL_BLENDMODE_BLEND);

		if (sprite->color.a == 50) {
			std::cout << "";
		}

		Helper::SDL_RenderCopyEx(
			0,
			"",
			renderer,
			sprite->texture,
			sprite->sourceRect,
			sprite->destinationRect,
			sprite->rotationDegrees,
			sprite->center,
			sprite->flip);

		SDL_SetTextureBlendMode(sprite->texture, SDL_BLENDMODE_NONE);


#if defined(RENDER_LOG)
		std::cout << Helper::GetFrameNumber() << ":"
			<< "-1: dstrect " << sprite->destinationRect->x
			<< " " << sprite->destinationRect->y
			<< " " << sprite->destinationRect->w
			<< " " << sprite->destinationRect->h
			<< " angle " << sprite->rotationDegrees
			<< " center " << sprite->center->x
			<< " " << sprite->center->y << " flip "
			<< sprite->flip << " renderscale "
			<< "1 1" << std::endl;
#endif

	}
}

void VengefulGameEngine::WindowManager::renderUI(SDL_Renderer* renderer)
{
	for (Sprite*& sprite : imagesManager.uiSprites) {

		sprite->destinationRect->w = static_cast<int>(sprite->destinationRect->w * globalCamera.zoomFactorInverse);
		sprite->destinationRect->h = static_cast<int>(sprite->destinationRect->h * globalCamera.zoomFactorInverse);
		sprite->destinationRect->x = static_cast<int>(sprite->destinationRect->x * globalCamera.zoomFactorInverse);
		sprite->destinationRect->y = static_cast<int>(sprite->destinationRect->y * globalCamera.zoomFactorInverse);

		SDL_SetTextureColorMod(sprite->texture,
			sprite->color.r, sprite->color.g, sprite->color.b);
		SDL_SetTextureAlphaMod(sprite->texture,
			sprite->color.a);

		Helper::SDL_RenderCopyEx(
			0,
			"",
			renderer,
			sprite->texture,
			sprite->sourceRect,
			sprite->destinationRect,
			sprite->rotationDegrees,
			sprite->center,
			sprite->flip);
	}
}

void VengefulGameEngine::WindowManager::renderText(SDL_Renderer* renderer)
{
	for (Sprite*& sprite : textManager.textSprites) {
		Helper::SDL_RenderCopyEx(
			0,
			"",
			renderer,
			sprite->texture,
			sprite->sourceRect,
			sprite->destinationRect,
			sprite->rotationDegrees,
			sprite->center,
			sprite->flip);
	}
}

void VengefulGameEngine::WindowManager::renderPixels(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	for (PixelDrawRequest*& pixelDrawRequests : globalPixelDrawRequests) {
		SDL_SetRenderDrawColor(renderer,
			pixelDrawRequests->color.r,
			pixelDrawRequests->color.g,
			pixelDrawRequests->color.b,
			pixelDrawRequests->color.a);
		SDL_RenderDrawPoint(renderer,
			pixelDrawRequests->position.x,
			pixelDrawRequests->position.y);
	}
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
	globalPixelDrawRequests.clear();
}

SDL_Renderer* VengefulGameEngine::WindowManager::getRenderer()
{
	return renderer;
}
