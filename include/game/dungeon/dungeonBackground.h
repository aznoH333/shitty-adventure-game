#ifndef DUNGEON_BACKGROUND
#define DUNGEON_BACKGROUND

#include <string>
#include <vector>


namespace DungeonCode{

    class BackgroundLayer{
        private:
            std::vector<std::string> contents;
            float paralax;
            int patternWidth;
        public:
            BackgroundLayer(float paralax);
            void addPattern(std::string pattern);
            void draw(float cameraOffset);
    };


    const int BACKGROUND_PATTERN_SIZE = 20;
    const int BACKGROUND_WIDTH = 96;



    class DungeonBackground{

        private:
            std::vector<BackgroundLayer> layers;
            void addLayer(BackgroundLayer layer);
            DungeonBackground();

        public:
            void draw(float cameraOffset);
            static DungeonBackground generateBasedOnTheme(int themeId);
            

    };
}


#endif