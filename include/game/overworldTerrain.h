#ifndef OVERWORLD_TERRAIN
#define OVERWORLD_TERRAIN

#include <vector>
#include <string>
#include "engine/all.h"

const float OVERWORLD_TILE_SIZE = 32.0f;
const int OVERWORLD_CHUNK_SIZE = 16;
const int LOADED_CHUNK_COUNT = 9;

namespace TerrainGeneration{


    // terrain variables
    const int terrainNoiseResolution = 4;


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

        bool operator==(const ChunkCoordinates &other) const {
            return x == other.x && y == other.y;
        }

        bool operator<(const ChunkCoordinates &other) const {
            return x < other.x || (x == other.x && y < other.y);
        }

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

    enum PatternGenrationType{
        PATTERN_WATER,
    };


    struct PatternGenerationObject{
        OverworldPosition position;
        PatternGenrationType patern;
    };


    struct OverworldChunk{
        ChunkCoordinates coordinates;
        int tiles[OVERWORLD_CHUNK_SIZE][OVERWORLD_CHUNK_SIZE];
        std::vector<OverworldObject> worldObjects;
        std::vector<PatternGenerationObject> patterns;
    };


    const OverworldTile tileLookupTable[] = {
        {"placeholders_1", false}, // grass
        {"placeholders_2", false}, // sand
        {"placeholders_3", true},  // mountain
        {"placeholders_4", true}   // water
    };

    struct TerrainBiome{
        float mountainValue;
        float stoneValue;
        float shoreValue;
        float waterGenerationValue;
        float treeGenerationValue;
        float structureFrequency;



        int mountainTile;
        int stoneTile;
        int shoreTile;
        int defaultTile;
        int sandTile;
        int structureType;
    };



    const TerrainBiome biomeLookup[] = {
        // default biome
        {
            .mountainValue = 0.7,
            .stoneValue = 0.6,
            .shoreValue = 0.4,
            .waterGenerationValue = 0.3,
            .treeGenerationValue = 0.7,
            .structureFrequency = 20,


            .mountainTile = 2,
            .stoneTile = 0,
            .shoreTile = 0,
            .defaultTile = 0,
            .sandTile = 1,
            .structureType = 0,
        }
    };

    class ValueNoiseMap{
        private:
            std::map<OverworldPosition, float> noise;
            int noiseSeed;
            Utils::SeededGenerator* generator;
            void generateNoiseValue(OverworldPosition position);
        public:
            float getNoiseValue(OverworldPosition& position, int resolution);
            ValueNoiseMap(int noiseSeed);
            ~ValueNoiseMap();
    };

    struct NoiseSpot {
        OverworldPosition position;
        float radius;
    };


    class SpotNoiseMap{
        private:
            std::map<ChunkCoordinates, NoiseSpot> noise; 
            int noiseSeed;
            Utils::SeededGenerator* generator;
            void generateNoiseValue(ChunkCoordinates position);
        public:
            float getNoiseValue(OverworldPosition& position, float minNoiseRadius, float maxNoiseRadius);
            SpotNoiseMap(int noiseSeed);
            ~SpotNoiseMap();
    };


    


    class OverworldTerrain{
        private:
            OverworldChunk* loadedChunks[LOADED_CHUNK_COUNT];
            ValueNoiseMap* noiseMap;
            SpotNoiseMap* mountainNoiseMap;
            
            
            void draw();
            void drawChunk(OverworldChunk* chunk);
            void addGenerationPatternsForChunk(std::vector<PatternGenerationObject>& patterns, ChunkCoordinates position);
            void addGenerationPattern(std::vector<PatternGenerationObject>& patterns, float terrainHeightValue, OverworldPosition position);
            OverworldChunk* generateChunk(ChunkCoordinates position);
            int generateTile(float terrainHeightValue);
            void applyPattern(int tiles[OVERWORLD_CHUNK_SIZE][OVERWORLD_CHUNK_SIZE], PatternGenerationObject& pattern, ChunkCoordinates& chunkPosition);

        public:
            void update();
            ~OverworldTerrain();
            OverworldTerrain();


    };

}

#endif