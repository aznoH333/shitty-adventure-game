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




    struct GenerationInfo{
        int ceilingHeight;
        int floorHeight;
        bool isGap;
        bool isWall;
        int minLength;
        int maxLength;
    };


    struct DungeonPattern{
        int tiles[TILES_PER_PATTERN];
        GenerationInfo generationInfo;
        SpawnInfo spawnInfo;
    };


    


    const std::vector<DungeonPattern> dungeonPatternLookup = {
        // full wall
        {
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
            {0,0, false, true, 0, 0},
            {NONE, 0},
        },
        
        // gap
        {
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1,14, true, false, 3, 6},
            {NONE, 0},
        },
        // gap platform
        {
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1,15, true, false, 5, 5},
            {PLATFORM, 10},
        },
        // gap water fall
        {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0,14, true, false, 5, 5},
            {WATER_FALL_PLATFORM_AND_PLATFORM, 10},
        },
        // gap 1/3
        {
            {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {4,14, true, false, 3, 6},
            {NONE, 0},
        },

        // gap 2/3
        {
            {1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
            {7,14, true, false, 2, 4},
            {NONE, 0},
        },


        // solid full
        {
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
            {1,12, false, false, 3, 7},
            {GENERIC, 2},
        },

        // solid 1/3
        {
            {1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1},
            {2,9, false, false, 3, 7},
            {GENERIC, 5},
        },

        // solid 2/3
        {
            {1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
            {2,6, false, false, 3, 7},
            {GENERIC, 8},
        },


    };

}

#endif