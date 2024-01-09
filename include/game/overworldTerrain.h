#ifndef OVERWORLD_TERRAIN
#define OVERWORLD_TERRAIN

#include <vector>
#include <string>
#include "engine/all.h"

const float OVERWORLD_TILE_SIZE = 32.0f;
const int OVERWORLD_CHUNK_SIZE = 16;
const int LOADED_CHUNK_COUNT = 9;


struct OverworldPosition{
    bool operator==(const OverworldPosition &other) const {
        return x == other.x && y == other.y;
    }

    bool operator<(const OverworldPosition &other) const {
        return x < other.x || (x == other.x && y < other.y);
    }

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

class NoiseMap{
    private:
        std::map<OverworldPosition, float> noise;
        int noiseSeed;
        int noiseResolution;
        Utils::SeededGenerator* generator;

        void generateNoiseValue(OverworldPosition position);
    public:
        float getNoiseValue(OverworldPosition& position);
        NoiseMap(int noiseSeed, int noiseResolution);
        ~NoiseMap();
};


class OverworldTerrain{
    private:
        OverworldChunk* loadedChunks[LOADED_CHUNK_COUNT];
        NoiseMap* noiseMap;
        
        
        void draw();
        void drawChunk(OverworldChunk* chunk);

        OverworldChunk* generateChunk(ChunkCoordinates position);
        int generateTile(OverworldPosition tilePosition);

    public:
        void update();
        ~OverworldTerrain();
        OverworldTerrain();


};



#endif