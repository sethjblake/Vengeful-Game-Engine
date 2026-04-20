#include "TextManager.h"
#include <string>
#include <iostream>
#include <cstdlib>
#include "../ContentManagement/ContentLoader.h"
#include "../../ThirdParty/SDL2/include/SDL_ttf.h"
#include "../../ThirdParty/SDL2/include/SDL_image.h"
#include "GlobalTextRequests.h"
#include "TextRequest.h"
#include "Sprite.h"
#include "EngineConfig/Macros.h"

void VengefulGameEngine::TextManager::init()
{
	TTF_Init();
}

void VengefulGameEngine::TextManager::generateTextures(SDL_Renderer* renderer, VengefulGameEngine::ContentLoader& contentLoader)
{
	while (!globalTextRequests.empty()) {

		TextRequest textRequest = globalTextRequests.front();
		globalTextRequests.pop();

		SDL_Color color = { textRequest.r, textRequest.g, textRequest.b, textRequest.a };
		std::string textContent = textRequest.textContent;

		TTF_Font* font = getFont(textRequest.fontName, textRequest.fontSize, contentLoader);

		if (!font) {
			std::cout << "error: font " << textRequest.fontName << " failed to load";
			exit(0);
		}

		SDL_Surface* surface = TTF_RenderText_Solid(font, textContent.c_str(), color);
		if (!surface) {
			std::cout << "error: Text surface failed to generate";
			exit(0);
		}
		SDL_Texture* texture = nullptr;
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		if (!texture) {
			std::cout << "error: Text texture failed to generate";
			exit(0);
		}

		Sprite* sprite = new Sprite();
		sprite->texture = texture;
		sprite->sourceRect = nullptr;
		sprite->destinationRect = new SDL_FRect();
		sprite->destinationRect->x = textRequest.x;
		sprite->destinationRect->y = textRequest.y;
		Helper::SDL_QueryTexture(texture, &sprite->destinationRect->w, &sprite->destinationRect->h);

		sprite->rotationDegrees = 0.0f;

		sprite->center = new SDL_FPoint();
		sprite->center->x = sprite->destinationRect->x;
		sprite->center->y = sprite->destinationRect->y;

		sprite->flip = SDL_FLIP_NONE;

		textSprites.push_back(sprite);

		SDL_FreeSurface(surface);
	}
}

void VengefulGameEngine::TextManager::clearTextures()
{
	for (Sprite*& spritePtr : textSprites) {
		if (spritePtr != nullptr) {
			if (spritePtr->destinationRect != nullptr) {
				delete spritePtr->destinationRect;
				spritePtr->destinationRect = nullptr;
			}
			delete spritePtr;
			spritePtr = nullptr;
		}
	}
	textSprites.clear();
}

TTF_Font* VengefulGameEngine::TextManager::loadFont(std::string fontName, int fontSize, VengefulGameEngine::ContentLoader& contentLoader)
{
	std::string resourcesFolder = RESOURCES_FOLDER;
	std::string fontFilePath = resourcesFolder + "/fonts";
	if (!contentLoader.filePathExists(fontFilePath)) {
		std::cout << "error: font " << fontName << " missing";
		exit(0);
	}
	std::string filePath = resourcesFolder + "/fonts/" + fontName + ".ttf";

	if (!contentLoader.filePathExists(filePath)) {
		std::cout << "error: font " << fontName << " missing";
		exit(0);
	}

	TTF_Font* font = TTF_OpenFont(filePath.c_str(), fontSize);
	if (!font) {
		std::cout << "error: font failed to open";
		exit(0);
	}

	fontMap[fontName][fontSize] = font;

	return font;
}

TTF_Font* VengefulGameEngine::TextManager::getFont(std::string fontName, int fontSize, VengefulGameEngine::ContentLoader& contentLoader)
{
	if (fontMap.find(fontName) != fontMap.end()) {
		if (fontMap[fontName].find(fontSize) != fontMap[fontName].end()) {
			return fontMap[fontName][fontSize];
		}
	}

	return loadFont(fontName, fontSize, contentLoader);
}

VengefulGameEngine::TextManager::~TextManager()
{

	for (auto& textureMapPair : textureMap) {
		SDL_DestroyTexture(textureMapPair.second);
	}
}
