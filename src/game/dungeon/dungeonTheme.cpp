#include "game/dungeon/dungeonTheme.h"
#include "engine/utils.h"

using namespace Utils;
namespace DungeonCode {
    DungeonTheme::DungeonTheme(std::string namePrefix, int defaultVariations){
        this->tileLookup = std::vector<DungeonTile>();
        this->defaultVariations = defaultVariations;
        // add air
        tileLookup.push_back({"", false, false});
        
        // add tiles
        int i = 1;
        while (i <= defaultVariations * 2 + 1){
            tileLookup.push_back({concatSprite(namePrefix.c_str(), i), true, false});
            i++;
        }
        // add spikes
        tileLookup.push_back({concatSprite(namePrefix.c_str(), i), true, true});

    }
    DungeonTile& DungeonTheme::getTile(const int& tileId){
        return tileLookup[tileId];
    }

    int DungeonTheme::translateTileId(const int& type){
        switch (type) {
            default:
            case 0:
                return 0;
            case 1:
                return GetRandomValue(1, defaultVariations);
            case 2:
                return GetRandomValue(1, defaultVariations) + defaultVariations;
            case 3:
                return defaultVariations * 2 + 1;
            case 4:
                return defaultVariations * 2 + 2;
        }
    }
}