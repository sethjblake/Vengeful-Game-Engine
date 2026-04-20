#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "ContentManagement/ContentLoader.h"
#include "SDL2/include/SDL_mixer.h"
#include "SDL2/include/SDL_ttf.h"

namespace VengefulGameEngine {
	class AudioManager {
	public:
		void init();
		void loadAudioFromSet(std::unordered_set<std::string> fileNames, VengefulGameEngine::ContentLoader& contentLoader);
		void loadAudio(std::string fileName, VengefulGameEngine::ContentLoader& contentLoader);
		void loadAudio(std::string fileName);
		void play(std::string& clip, int channel, bool loop);
		void halt(int channel);
		void setVolume(int channel, int volume);
		~AudioManager();

	private:
		std::unordered_map<std::string, Mix_Chunk*> audioClipMap;
		std::string directoryPath;
	};
}

#endif