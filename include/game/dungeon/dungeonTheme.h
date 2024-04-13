#ifndef DUNGEON_THEME
#define DUNGEON_THEME

#include "game/dungeon/dungeonTileLookup.h"
#include <string>
#include <map>

namespace DungeonCode {
    enum DungeonThemeType{
        THEME_DEFAULT,
        THEME_ALT,
        THEME_GREEN,
        THEME_MUD,
        THEME_SAND,
    };
   
    
    class DungeonTheme{
        private:
            std::string namePrefix;
            int defaultVariations;
            std::vector<DungeonTile> tileLookup;
            std::string overworldSprite;
            DungeonThemeType id;
        
        public:
            DungeonTheme(DungeonThemeType id, std::string namePrefix, int defaultVariations, std::string overworldSprite);
            DungeonTile& getTile(const int& tileId);
            int translateTileId(const int& type);
            std::string getOverworldSprite();
            int getId();
    };


    extern std::map<DungeonThemeType, DungeonTheme> themeLookup;

}
#endif