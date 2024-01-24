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
#include "biomeNoiseMap.h"
#include <chrono>

const float OVERWORLD_TILE_SIZE = 16.0f;
const int DISPLAYED_CHUNK_COUNT = 9;
const int CHUNK_GENERATION_DISTANCE = 3;


namespace TerrainGeneration{
    const int DEFAULT_THREAD_WAIT = 50;
    const int DEFAULT_WORLD_LOADING_INTERVAL = 10;


    const int MOUNTAIN_NOISE_RESOLUTION = 16;
    const int TREE_NOISE_RESOLUTION = 8;
    const int HEIGHT_NOISE_RESOLUTION = 16;
    const int BIOME_SIZE = 96;

    

    class OverworldTerrain{
        private:
            OverworldChunk* displayedChunks[DISPLAYED_CHUNK_COUNT];
            std::map<ChunkCoordinates, OverworldChunk*> loadedChunks;
            ChunkCoordinates lastLoadChunk = {0,0};
            BiomeNoiseMap* biomeMap;
            ValueNoiseMap* noiseMap;
            ValueNoiseMap* treeNoiseMap;
            SpotNoiseMap* mountainNoiseMap;
            SpotNoiseMap* structureNoiseMap;
            std::thread worldLoadingThread;

            bool shouldAttemptLoading = true;
            bool initialLoading = true;
            bool allChunksLoaded = false;
            bool shouldWorldLoadingThreadBeRunning = true;            
            
            void worldLoadingFunction();
            void generateChunksAroundPoint(ChunkCoordinates coordinates);
            void draw();
            void doneLoading();
            void drawChunk(OverworldChunk* chunk);
            void addGenerationPatternsForChunk(std::vector<PatternGenerationObject>& patterns, ChunkCoordinates position);
            void addGenerationPattern(std::vector<PatternGenerationObject>& patterns, ChunkCoordinates& chunkPosition, int x, int y);
            OverworldChunk* generateChunk(ChunkCoordinates position);
            int generateTile(float terrainHeightValue, const TerrainBiome& biome);
            void applyPattern(int tiles[OVERWORLD_CHUNK_SIZE][OVERWORLD_CHUNK_SIZE], std::vector<OverworldObject>& objects, PatternGenerationObject& pattern, ChunkCoordinates& chunkPosition);
            OverworldObject generateOverworldObject(const TerrainBiome& biome, ObjectType type, OverworldPosition position);
            float getTerrainHeightValue(OverworldPosition position, const TerrainBiome& biome);
            void drawTile(const OverworldTile& tile, Vector2& position);
        public:
            void update();
            void chunkLoadingUpdate();
            void reloadChunksAroundPoint(ChunkCoordinates coordinates);
            bool isTileBlocking(ChunkCoordinates chunkPosition, int x, int y);
            bool isDoneLoading();
            std::vector<OverworldObject*> getNearbyObjects(OverworldPosition position, float radius);
            ~OverworldTerrain();
            OverworldTerrain();


    };

}

#endif