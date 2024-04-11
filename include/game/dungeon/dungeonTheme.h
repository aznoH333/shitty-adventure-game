#ifndef DUNGEON_THEME
#define DUNGEON_THEME

#include "game/dungeon/dungeonTileLookup.h"
#include <string>

namespace DungeonCode {
    
   
    
    class DungeonTheme{
        private:
            std::string namePrefix;
            int defaultVariations;
            std::vector<DungeonTile> tileLookup;
        
        public:
            DungeonTheme(std::string namePrefix, int defaultVariations);
            DungeonTile& getTile(const int& tileId);
            int translateTileId(const int& type);
    };
}



#endif