#ifndef DUNGEON_TILE_LOOKUP
#define DUNGEON_TILE_LOOKUP

#include <string>

const int TILES_PER_PATTERN = 12;

struct DungeonTile{
    std::string sprite;
    bool isSolid;
};

const DungeonTile dungeonTileLookup[] = {
    {"", false}, // air
    {"dungeon_test_1", true}, // default bricks
};


struct DungeonPattern{
    int tiles[TILES_PER_PATTERN];
    int minLength;
    int maxLength;
    int connectorGroupId;
};

struct Connector{
    int count;
    int patternIds[8];
};

// contains ids of possible next tiles
const Connector connectorGroupLookup[]{
    {7, {1, 2, 3, 4, 5, 6, 7, 8}},
    {4, {1, 2, 5, 4, 8}},
    {4, {1, 3, 4, 6, 7}},
    {6, {1, 2, 3, 4, 7, 8}},
    {3, {1, 3, 4, 7}},


};


const DungeonPattern dungeonPatternLookup[] = {
    {{1, 1 ,1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 5, 5, 0},
    {{1, 1 ,0, 0, 0, 0, 0, 0, 0, 1, 1, 1}, 3, 5, 0},
    {{1, 1 ,1, 1, 1, 0, 0, 0, 0, 1, 1, 1}, 3, 5, 1},
    {{1, 1 ,0, 0, 0, 1, 1, 1, 1, 1, 1, 1}, 3, 5, 2},
    {{1, 1 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 3, 5, 3},
    {{1, 1 ,1, 1, 1, 0, 0, 0, 0, 0, 0, 0}, 3, 5, 4},
    {{1, 1 ,0, 0, 0, 1, 0, 0, 0, 1, 1, 1}, 3, 5, 2},
    {{1, 1 ,0, 0, 0, 1, 0, 0, 0, 0, 0, 0}, 3, 5, 2},
    {{1, 1 ,1, 1, 1, 0, 0, 0, 0, 1, 0, 0}, 3, 5, 1},




};

#endif