#ifndef DUNGEON_TILE_LOOKUP
#define DUNGEON_TILE_LOOKUP

#include <string>
#include <vector>
namespace DungeonCode{
const int TILES_PER_PATTERN = 14;

    struct DungeonTile{
        std::string sprite;
        bool isSolid;
    };

    const DungeonTile dungeonTileLookup[] = {
        {"", false}, // air
        {"dungeon_test_1", true}, // default bricks
    };


    enum SpawnType{
        NONE,
        WATER_FALL_PLATFORM_AND_PLATFORM,
        PLATFORM,
        GENERIC,
    };

    struct SpawnInfo{
        SpawnType spawnType = NONE;
        int spawnY = 0;
    };


    struct DungeonPattern{
        int tiles[TILES_PER_PATTERN];
        int minLength;
        int maxLength;
        std::vector<int> connectors;
        SpawnInfo spawnInfo;
    };


    


    const DungeonPattern dungeonPatternLookup[] = {
        {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 5, 5, {4, 5, 6, 7, 8}, {NONE, 0}}, // full wall [0]

        // duplicate gap??
        {{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 3, 5, {4, 5, 6, 7, 8, 9, 10, 11, 12}, {NONE, 0}}, // gaps [1-3]
        {{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 3, 5, {4, 5, 6, 7, 8, 9, 10, 11, 12}, {NONE, 10}},
        {{1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 3, 5, {4, 6, 7}, {NONE, 0}},

        
        {{1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1}, 3, 5, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, {GENERIC, 6}}, // solid full [4-8]
        {{1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 3, 5, {1, 2, 4, 5, 6, 8}, {GENERIC, 10}}, // solid upper
        {{1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1}, 3, 5, {1, 2, 4, 5, 6, 7, 8}, {GENERIC, 8}}, // solid transitional
        {{1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1}, 3, 5, {1, 2, 3, 4, 6, 7}, {GENERIC, 6}}, // solid tunnel
        {{1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 3, 5, {1, 2, 4, 5, 6, 8}, {GENERIC, 10}}, // platform upper 

        {{1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1}, 3, 5, {1, 2, 4, 9, 10}, {GENERIC, 10}}, // fork [9-10]
        {{1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1}, 3, 5, {1, 2, 4, 9, 10}, {GENERIC, 6}}, // fork lower spawn

        {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 5, 5, {1, 2, 4, 5, 6, 8, 9, 10}, {WATER_FALL_PLATFORM_AND_PLATFORM, 8}}, // gaps with platforms [11-12]
        {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 5, 5, {1, 2, 4, 5, 6, 8, 9, 10}, {PLATFORM, 8}},



    };

}

#endif