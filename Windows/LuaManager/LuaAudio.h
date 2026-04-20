#ifndef LUA_AUDIO_H
#define LUA_AUDIO_H

#include <string>
#include "../AudioSystem/GlobalAudioManager.h"

namespace LuaAudio {

	void Play(int channel, std::string clipName, bool doesLoop) {
		VengefulGameEngine::globalAudioManager.play(clipName, channel, doesLoop);
	}
	
	void Halt(int channel) {
		VengefulGameEngine::globalAudioManager.halt(channel);
	}
	
	void SetVolume(int channel, float volume) {
		VengefulGameEngine::globalAudioManager.setVolume(channel, static_cast<int>(volume));
	}
}

#endif