#include "AudioManager.h"
#include <cstdlib>
#include <iostream>
#include <unordered_map>
#include <filesystem>
#include <AudioHelper.h>
#include <SDL_mixer.h>
#include "EngineConfig/Macros.h"

void VengefulGameEngine::AudioManager::init() {
    
    if (AudioHelper::Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
        std::cout << "error: audio mixer failed to load";
        exit(0);
    }

    AudioHelper::Mix_AllocateChannels(50);

    std::string resourcesFolder = RESOURCES_FOLDER;

    directoryPath = resourcesFolder + "/audio";

}

void VengefulGameEngine::AudioManager::loadAudioFromSet(std::unordered_set<std::string> fileNames, VengefulGameEngine::ContentLoader& contentLoader)
{
    if (!contentLoader.filePathExists(directoryPath)) {
        std::cout << "error: " << directoryPath << " missing";
        exit(0);
    }

    for (std::string fileName : fileNames) {
        if (contentLoader.fileExists(directoryPath, fileName + ".wav")) {
            std::string filePath = directoryPath + "/" + fileName + ".wav";
            audioClipMap[fileName] = AudioHelper::Mix_LoadWAV(filePath.c_str());
        }
        else if (contentLoader.fileExists(directoryPath, fileName + ".ogg")) {
            std::string filePath = directoryPath + "/" + fileName + ".ogg";
            audioClipMap[fileName] = AudioHelper::Mix_LoadWAV(filePath.c_str());
        }
    }
}

void VengefulGameEngine::AudioManager::loadAudio(std::string fileName, VengefulGameEngine::ContentLoader& contentLoader)
{
    if (!contentLoader.filePathExists(directoryPath)) {
        std::cout << "error: " << directoryPath << " missing";
        exit(0);
    }

    if (contentLoader.fileExists(directoryPath, fileName + ".wav")) {
        std::string filePath = directoryPath + "/" + fileName + ".wav";
        audioClipMap[fileName] = AudioHelper::Mix_LoadWAV(filePath.c_str());
    }
    else if (contentLoader.fileExists(directoryPath, fileName + ".ogg")) {
        std::string filePath = directoryPath + "/" + fileName + ".ogg";
        audioClipMap[fileName] = AudioHelper::Mix_LoadWAV(filePath.c_str());
    }
}

void VengefulGameEngine::AudioManager::loadAudio(std::string fileName)
{
    if (!std::filesystem::exists(directoryPath)) {
        std::cout << "error: " << directoryPath << " missing";
        exit(0);
    }

    if (std::filesystem::exists(directoryPath + "/" + fileName + ".wav")) {
        std::string filePath = directoryPath + "/" + fileName + ".wav";
        audioClipMap[fileName] = AudioHelper::Mix_LoadWAV(filePath.c_str());
    }
    else if (std::filesystem::exists(directoryPath + "/" + fileName + ".ogg")) {
        std::string filePath = directoryPath + "/" + fileName + ".ogg";
        audioClipMap[fileName] = AudioHelper::Mix_LoadWAV(filePath.c_str());
    }
}

void VengefulGameEngine::AudioManager::play(std::string& clip, int channel, bool loop)
{
    if (audioClipMap.find(clip) == audioClipMap.end()) {
        loadAudio(clip);
    }

    Mix_Chunk* audioClip = audioClipMap[clip];

    if (audioClip != NULL) {
        int loopValue = (loop) ? -1 : 0;
        AudioHelper::Mix_PlayChannel(channel, audioClip, loopValue);
    }
    else {
        std::cout << "error: audio clip " << clip << " failed to play.";
        exit(0);
    }
}

void VengefulGameEngine::AudioManager::halt(int channel)
{
    AudioHelper::Mix_HaltChannel(channel);
}

void VengefulGameEngine::AudioManager::setVolume(int channel, int volume)
{
    AudioHelper::Mix_Volume(channel, volume);
}

VengefulGameEngine::AudioManager::~AudioManager()
{
    AudioHelper::Mix_CloseAudio();
    Mix_Quit();
}
