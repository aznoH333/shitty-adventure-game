#ifndef SOUND_MANAGER
#define SOUND_MANAGER

#include <string>
#include <map>
#include "raylib.h"
#include "engine/utils.h"


namespace Audio{


    enum SoundChannel{
        GAME,
        MUSIC,
        INTERFACE,
    };

    class SoundManager{
        private:
            std::map<std::string, Sound> loadedSounds;

            float maxPitchRandomization = 0.15f;
            std::map<SoundChannel, float> soundChannelSettings = {
                {GAME, 0.5},
                {MUSIC, 0.5},
                {INTERFACE, 0.5},
            };

            void loadSound(std::string path);

        public:
            void loadAllSounds(std::string soundsPath);
            void playSound(std::string name);
            void playSound(std::string name, float volume);
            void playSound(std::string name, float volume, float pitch ,SoundChannel channel);
            void setPitchRandomization(int number);
            SoundManager();
            ~SoundManager();
    };
    SoundManager* get();
    void dispose();


}

#endif