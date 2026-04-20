#include "ImagesManager.h"
#include <string>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include "SDL2/include/SDL_image.h"
#include "SDL2/include/SDL.h"
#include "ContentManagement/ContentLoader.h"
#include "ImageDrawRequest.h"
#include "GlobalImageDrawRequests.h"
#include "EngineConfig/Macros.h"

// #define QUICK_LOAD

void VengefulGameEngine::ImagesManager::loadTexturesFromSet(SDL_Renderer* renderer, VengefulGameEngine::ContentLoader& contentLoader, std::string& path, std::unordered_set<std::string>& files)
{
	for (const std::string& file : files) {

		std::string fileNamePNG = file + ".png";

		std::string filePath = path + "/" + fileNamePNG;

		if (!contentLoader.filePathExists(filePath)) {
			std::cout << "error: missing image " << file;
			exit(0);
		}

		SDL_Texture* texture = nullptr;
		texture = IMG_LoadTexture(renderer, filePath.c_str());
		if (texture == nullptr) {
			std::cout << "error: path " << filePath << " does not lead to a texture";
			exit(0);
		}

		textureMap[file] = texture;
	}
}

void VengefulGameEngine::ImagesManager::generateImages(SDL_Renderer* renderer, VengefulGameEngine::ContentLoader& contentLoader)
{
	std::stable_sort(globalImageDrawRequests.begin(), globalImageDrawRequests.end(),
		[](const ImageDrawRequest* a, const ImageDrawRequest* b) {
			return a->sortingOrder < b->sortingOrder;
		});

	for (ImageDrawRequest* drawRequest : globalImageDrawRequests) {

		Sprite* sprite = new Sprite();

		if (drawRequest->imageName.rfind("VengefulGameEngineDefaultParticle") == 0) {
			if (textureMap.find(drawRequest->imageName) != textureMap.end()) {
				sprite->texture = textureMap[drawRequest->imageName];
			}
			else {
				sprite->texture = createParticleTexture(drawRequest->imageName, renderer, 8, 8, drawRequest->color);
			}
		}
		else {
			if (textureMap.find(drawRequest->imageName) == textureMap.end()) {
				loadTexture(drawRequest->imageName, renderer, contentLoader);
			}

			sprite->texture = textureMap[drawRequest->imageName];
		}

#if defined(QUICK_LOAD)
		sprite->texture = quickLoadTexture(drawRequest->imageName, renderer, contentLoader);
#endif

		sprite->color = drawRequest->color;

		Helper::SDL_QueryTexture(sprite->texture,
			&sprite->colliderWidth,
			&sprite->colliderHeight);

		sprite->sourceRect = nullptr;

		sprite->destinationRect = new SDL_FRect();
		sprite->destinationRect->x = drawRequest->position.x - drawRequest->pivot.x * sprite->colliderWidth * drawRequest->scale.x;
		sprite->destinationRect->y = drawRequest->position.y - drawRequest->pivot.y * sprite->colliderHeight * drawRequest->scale.y;
		sprite->destinationRect->w = static_cast<int>(sprite->colliderWidth * drawRequest->scale.x);
		sprite->destinationRect->h = static_cast<int>(sprite->colliderHeight * drawRequest->scale.y);

		sprite->rotationDegrees = drawRequest->rotationDegrees;

		sprite->center = new SDL_FPoint();
		sprite->center->x = drawRequest->pivot.x * sprite->colliderWidth * drawRequest->scale.x;
		sprite->center->y = drawRequest->pivot.y * sprite->colliderHeight * drawRequest->scale.y;

		sprite->flip = SDL_FLIP_NONE;
		if (drawRequest->scale.x < 0) {
			sprite->flip = SDL_FLIP_HORIZONTAL;
		}
		if (drawRequest->scale.y < 0) {
			sprite->flip = SDL_FLIP_VERTICAL;
		}

		imageSprites.push_back(sprite);

	}

	globalImageDrawRequests.clear();
}

void VengefulGameEngine::ImagesManager::generateUI(SDL_Renderer* renderer, VengefulGameEngine::ContentLoader& contentLoader)
{
	std::stable_sort(globalImageUIDrawRequests.begin(), globalImageUIDrawRequests.end(),
		[](const ImageDrawRequest* a, const ImageDrawRequest* b) {
			return a->sortingOrder < b->sortingOrder;
		});

	for (ImageDrawRequest* drawRequest : globalImageUIDrawRequests) {

		Sprite* sprite = new Sprite();

		if (textureMap.find(drawRequest->imageName) == textureMap.end()) {
			loadTexture(drawRequest->imageName, renderer, contentLoader);
		}

		sprite->texture = textureMap[drawRequest->imageName];

#if defined(QUICK_LOAD)
		sprite->texture = quickLoadTexture(drawRequest->imageName, renderer, contentLoader);
#endif
		sprite->color = drawRequest->color;

		sprite->sourceRect = nullptr;
		sprite->destinationRect = new SDL_FRect();
		sprite->destinationRect->x = static_cast<int>(drawRequest->position.x);
		sprite->destinationRect->y = static_cast<int>(drawRequest->position.y);
		Helper::SDL_QueryTexture(sprite->texture,
			&sprite->destinationRect->w,
			&sprite->destinationRect->h);

		sprite->rotationDegrees = 0.0f;

		sprite->center = new SDL_FPoint();
		sprite->center->x = sprite->destinationRect->x;
		sprite->center->y = sprite->destinationRect->y;

		sprite->flip = SDL_FLIP_NONE;

		uiSprites.push_back(sprite);

	}
	globalImageUIDrawRequests.clear();
}

void VengefulGameEngine::ImagesManager::loadTexture(std::string imageName, SDL_Renderer* renderer, VengefulGameEngine::ContentLoader& contentLoader)
{

	std::string fileNamePNG = imageName + ".png";

	std::string resourcesFolder = RESOURCES_FOLDER;

	std::string filePath = resourcesFolder + "/images/" + fileNamePNG;

	if (!contentLoader.filePathExists(filePath)) {
		std::cout << "error: missing image " << imageName;
		exit(0);
	}

	SDL_Texture* texture = nullptr;
	texture = IMG_LoadTexture(renderer, filePath.c_str());
	if (texture == nullptr) {
		std::cout << "error: path " << filePath << " does not lead to a texture";
		exit(0);
	}

	textureMap[imageName] = texture;

}

SDL_Texture* VengefulGameEngine::ImagesManager::quickLoadTexture(std::string imageName, SDL_Renderer* renderer, ContentLoader& contentLoader)
{
	std::string fileNamePNG = imageName + ".png";

	std::string resourcesFolder = RESOURCES_FOLDER;

	std::string filePath = resourcesFolder + "/images/" + fileNamePNG;

	if (!contentLoader.filePathExists(filePath)) {
		std::cout << "error: missing image " << imageName;
		exit(0);
	}

	SDL_Texture* texture = nullptr;
	texture = IMG_LoadTexture(renderer, filePath.c_str());
	if (texture == nullptr) {
		std::cout << "error: path " << filePath << " does not lead to a texture";
		exit(0);
	}

	return texture;
}

SDL_Texture* VengefulGameEngine::ImagesManager::createParticleTexture(const std::string& name, SDL_Renderer* renderer, int width, int height, SDL_Color color)
{
	auto itr = textureMap.find(name);
	if (itr != textureMap.end()) {
		auto item = *itr;
		return item.second;
	}

	SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA8888);

	uint32_t formattedColor = SDL_MapRGBA(surface->format, color.r, color.g, color.b, 255);

	SDL_FillRect(surface, NULL, formattedColor);

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);
	textureMap[name] = texture;
	return texture;
}

void VengefulGameEngine::ImagesManager::clearImages()
{
	for (Sprite* sprite : imageSprites) {
		if (sprite != nullptr) {

			if (sprite->destinationRect != nullptr) {
				delete sprite->destinationRect;
				sprite->destinationRect = nullptr;
			}

#if defined(QUICK_LOAD)
			if (sprite->texture != nullptr) {
				SDL_DestroyTexture(sprite->texture);
				sprite->texture = nullptr;
			}
#endif

			delete sprite;
			sprite = nullptr;
		}
	}
	imageSprites.clear();
}

void VengefulGameEngine::ImagesManager::clearUI()
{
	for (Sprite* sprite : uiSprites) {
		if (sprite != nullptr) {

			if (sprite->destinationRect != nullptr) {
				delete sprite->destinationRect;
				sprite->destinationRect = nullptr;
			}

#if defined(QUICK_LOAD)
			if (sprite->texture != nullptr) {
				SDL_DestroyTexture(sprite->texture);
				sprite->texture = nullptr;
			}
#endif
			delete sprite;
			sprite = nullptr;
		}
	}
	uiSprites.clear();
}

void VengefulGameEngine::ImagesManager::clearTextureMap()
{
	for (auto& item : textureMap) {
		SDL_DestroyTexture(item.second);
	}

	textureMap.clear();
}

VengefulGameEngine::ImagesManager::~ImagesManager()
{
	for (auto& pear : textureMap) {
		SDL_DestroyTexture(pear.second);
	}
}
