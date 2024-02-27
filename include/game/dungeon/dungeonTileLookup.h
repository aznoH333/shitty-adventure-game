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
        {"dungeon_tiles_2", true}, // default bricks
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
            {0,0, false, true, 1, 2},
            {NONE, 0},
        },

        // tunnel 2
        {
            {1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
            {1, 6, false, false, 3, 7},
            {GENERIC, 9},
        },

        // tunnel 1
        {
            {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1},
            {8,12, false, false, 3, 7},
            {GENERIC, 2},
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
            {1,9, true, false, 5, 5},
            {PLATFORM, 6},
        },
        // gap water fall
        {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0,9, true, false, 5, 5},
            {WATER_FALL_PLATFORM_AND_PLATFORM, 5},
        },
        // gap 1/3
        {
            {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {4,14, true, false, 3, 5},
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

        // reward room step
        {
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1},
            {1,11, false, false, 3, 7},
            {GENERIC, 3},
        },

        // reward room step 2
        {
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1},
            {1,10, false, false, 3, 7},
            {GENERIC, 4},
        },


    };

}

#endif