#ifndef DUNGEON_TILE_LOOKUP
#define DUNGEON_TILE_LOOKUP

#include <string>

const int TILES_PER_PATTERN = 16;

struct DungeonTile{
    std::string sprite;
    bool isSolid;
};

const DungeonTile dungeonTileLookup[] = {
    {"", false}, // air
    {"dungeon_test_1", true}, // default bricks
};




const int dungeonPatternLookup[][TILES_PER_PATTERN] = {
    {1,1 ,1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1,1 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1},
    {1,1 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1,1 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
    {1,1 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1},
    {1,1 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0},

};

#endif