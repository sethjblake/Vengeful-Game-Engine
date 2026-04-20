#ifndef TEXT_MANAGER_H
#define TEXT_MANAGER_H

#include <string>
#include <vector>
#include <unordered_map>
#include "ContentManagement/ContentLoader.h"
#include "SDL2/include/SDL_image.h"
#include "SDL2/include/SDL_ttf.h"
#include "Sprite.h"

namespace VengefulGameEngine{
	class TextManager {
	public:
		void init();
		void generateTextures(SDL_Renderer* renderer, VengefulGameEngine::ContentLoader& contentLoader);
		void clearTextures();
		TTF_Font* loadFont(std::string fontName, int fontSize, VengefulGameEngine::ContentLoader& contentLoader);
		TTF_Font* getFont(std::string fontName, int fontSize, VengefulGameEngine::ContentLoader& contentLoader);
		~TextManager();
		std::vector<Sprite*> textSprites;

	private:
		std::unordered_map<std::string, std::unordered_map<int, TTF_Font*>> fontMap;
		std::unordered_map<std::string, SDL_Texture*> textureMap;

	};
}

#endif