#include "engine/sound.h"

namespace Audio {
    void SoundManager::loadSound(std::string path){
        std::string name = path.substr(path.find_last_of("/")+1);
        name = name.substr(0, name.find_last_of("."));   

        loadedSounds[name] = LoadSound(path.c_str());
    }

    void SoundManager::playSound(std::string name){
        playSound(name, 1);
    }

    void SoundManager::playSound(std::string name, float volume, float pitch ,SoundChannel channel){
        SetSoundVolume(loadedSounds[name], volume * soundChannelSettings[channel]);
        SetSoundPitch(loadedSounds[name], pitch);
        PlaySound(loadedSounds[name]);
    }

    void SoundManager::playSound(std::string name, float volume){
        playSound(name, volume, 1 + getRandomFloat(-maxPitchRandomization, maxPitchRandomization), GAME);
    }

    void SoundManager::setPitchRandomization(int number){
        this->maxPitchRandomization = number;
    }

    SoundManager::SoundManager(){
        
    }

    SoundManager::~SoundManager(){
        // unload sounds
        for (std::pair<std::string, Sound> pair : loadedSounds){
            UnloadSound(pair.second);
        }

        CloseAudioDevice();
    }
}
