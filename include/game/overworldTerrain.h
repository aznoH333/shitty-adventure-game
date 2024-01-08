#ifndef OVERWORLD_TERRAIN
#define OVERWORLD_TERRAIN

#include <vector>
#include <string>
#include "engine/all.h"

const float OVERWORLD_TILE_SIZE = 32.0f;
const int OVERWORLD_CHUNK_SIZE = 16;
const int LOADED_CHUNK_COUNT = 9;


struct OverworldPosition{
    int x;
    int y;
};

struct ChunkCoordinates{
    int x;
    int y;
};



struct OverworldTile{
    std::string sprite;
    bool blocksMovement;
};


// tree, castle, dungeon, village ect
struct OverworldObject{
    std::string sprite;
    OverworldPosition position;
};


struct OverworldChunk{
    ChunkCoordinates coordinates;
    int tiles[OVERWORLD_CHUNK_SIZE][OVERWORLD_CHUNK_SIZE];
    std::vector<OverworldObject> worldObjects;
};


const OverworldTile tileLookupTable[] = {
    {"placeholders_1", false},
    {"placeholders_2", false},
    {"placeholders_3", true},
    {"placeholders_4", true}
};


class OverworldTerrain{
    private:
        OverworldChunk* loadedChunks[LOADED_CHUNK_COUNT];
        
        
        void draw();
        void drawChunk(OverworldChunk* chunk);

        OverworldChunk* generateChunk(ChunkCoordinates position);

    public:
        void update();
        ~OverworldTerrain();
        OverworldTerrain();


};

#endif