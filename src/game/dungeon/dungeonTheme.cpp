#include "game/dungeon/dungeonTheme.h"
#include "engine/utils.h"

using namespace Utils;
namespace DungeonCode {
    DungeonTheme::DungeonTheme(DungeonThemeType id, std::string namePrefix, int defaultVariations, std::string overworldSprite){
        this->tileLookup = std::vector<DungeonTile>();
        this->defaultVariations = defaultVariations;
        this->overworldSprite = overworldSprite;
        this->id = id;
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

    std::string DungeonTheme::getOverworldSprite(){
        return overworldSprite;
    }

    int DungeonTheme::getId(){
        return id;
    }

    std::map<DungeonThemeType, DungeonTheme> themeLookup = {
        {THEME_DEFAULT, DungeonTheme(THEME_DEFAULT, "default_castle_", 2, "dungeon_2")},
        {THEME_ALT, DungeonTheme(THEME_ALT, "alt_castle_", 2, "dungeon_4")},
        {THEME_GREEN, DungeonTheme(THEME_GREEN, "green_castle_", 4, "dungeon_3")},
        {THEME_MUD, DungeonTheme(THEME_MUD, "mud_castle_", 2, "dungeon_5")},
        {THEME_SAND, DungeonTheme(THEME_SAND, "sand_castle_", 2, "dungeon_1")}
    };
}