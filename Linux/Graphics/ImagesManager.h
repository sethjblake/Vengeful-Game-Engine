#ifndef IMAGES_MANAGER_H
#define IMAGES_MANAGER_H

#include <string>
#include <vector>
#include "SDL2/Helper.h"
#include "SDL2/include/SDL_image.h"
#include "ContentManagement/ContentLoader.h"
#include "Sprite.h"
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <string>

namespace VengefulGameEngine {
	class ImagesManager {
	public:
		void loadTexturesFromSet(SDL_Renderer* renderer, ContentLoader& contentLoader, std::string& path, std::unordered_set<std::string>& file);
		void generateImages(SDL_Renderer* renderer, ContentLoader& contentLoader);
		void generateUI(SDL_Renderer* renderer, ContentLoader& contentLoader);
		void loadTexture(std::string imageName, SDL_Renderer* renderer, ContentLoader& contentLoader);
		SDL_Texture* quickLoadTexture(std::string imageName, SDL_Renderer* renderer, ContentLoader& contentLoader);
		SDL_Texture* createParticleTexture(const std::string& name, SDL_Renderer* renderer, int width, int height, SDL_Color color);
		void clearImages();
		void clearUI();
		std::vector<Sprite*> imageSprites;
		std::vector<Sprite*> uiSprites;
		std::unordered_map<std::string, SDL_Texture*> textureMap;
		void clearTextureMap();
		~ImagesManager();
	};
}

#endif