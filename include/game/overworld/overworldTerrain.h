#ifndef OVERWORLD_TERRAIN
#define OVERWORLD_TERRAIN

#include <thread>
#include <vector>
#include "engine/all.h"
#include "tileLookup.h"
#include "biomeLookup.h"
#include "spotNoise.h"
#include "terrainStructs.h"
#include "valueNoise.h"
#include <chrono>

const float OVERWORLD_TILE_SIZE = 16.0f;
const int DISPLAYED_CHUNK_COUNT = 9;
const int CHUNK_GENERATION_DISTANCE = 3;


namespace TerrainGeneration{
    const int DEFAULT_THREAD_WAIT = 50;
    const int DEFAULT_WORLD_LOADING_INTERVAL = 60;


    const int MOUNTAIN_NOISE_RESOLUTION = 16;
    const int TREE_NOISE_RESOLUTION = 16;
    const int HEIGHT_NOISE_RESOLUTION = 16;

    

    class OverworldTerrain{
        private:
            OverworldChunk* displayedChunks[DISPLAYED_CHUNK_COUNT];
            std::map<ChunkCoordinates, OverworldChunk*> loadedChunks;
            ChunkCoordinates lastLoadChunk = {0,0};
            ValueNoiseMap* noiseMap;
            ValueNoiseMap* treeNoiseMap;
            SpotNoiseMap* mountainNoiseMap;
            SpotNoiseMap* structureNoiseMap;
            std::thread worldLoadingThread;

            bool shouldAttemptLoading = true;
            bool allChunksLoaded = false;
            bool shouldWorldLoadingThreadBeRunning = true;            
            
            void worldLoadingFunction();
            void generateChunksAroundPoint(ChunkCoordinates coordinates);
            void draw();
            void drawChunk(OverworldChunk* chunk);
            void addGenerationPatternsForChunk(std::vector<PatternGenerationObject>& patterns, ChunkCoordinates position, const TerrainBiome& biome);
            void addGenerationPattern(std::vector<PatternGenerationObject>& patterns, ChunkCoordinates& chunkPosition, int x, int y, const TerrainBiome& biome);
            OverworldChunk* generateChunk(ChunkCoordinates position);
            int generateTile(float terrainHeightValue, const TerrainBiome& biome);
            void applyPattern(int tiles[OVERWORLD_CHUNK_SIZE][OVERWORLD_CHUNK_SIZE], std::vector<OverworldObject>& objects, PatternGenerationObject& pattern, ChunkCoordinates& chunkPosition, const TerrainBiome& biome);
            OverworldObject generateOverworldObject(const TerrainBiome& biome, ObjectType type, OverworldPosition position);
            float getTerrainHeightValue(OverworldPosition position);
        public:
            void update();
            void reloadChunksAroundPoint(ChunkCoordinates coordinates);
            ~OverworldTerrain();
            OverworldTerrain();


    };

}

#endif