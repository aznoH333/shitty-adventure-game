#ifndef OVERWORLD_TERRAIN
#define OVERWORLD_TERRAIN

#include <vector>
#include "engine/all.h"
#include "tileLookup.h"
#include "biomeLookup.h"
#include "spotNoise.h"
#include "terrainStructs.h"
#include "valueNoise.h"

const float OVERWORLD_TILE_SIZE = 32.0f;
const int DISPLAYED_CHUNK_COUNT = 9;
const int CHUNK_GENERATION_DISTANCE = 3;

namespace TerrainGeneration{


    // terrain variables
    const int terrainNoiseResolution = 4;
    const int treeNoiseResolution = 8;
    

    class OverworldTerrain{
        private:
            OverworldChunk* displayedChunks[DISPLAYED_CHUNK_COUNT];
            std::map<ChunkCoordinates, OverworldChunk*> loadedChunks;
            ValueNoiseMap* noiseMap;
            ValueNoiseMap* treeNoiseMap;
            SpotNoiseMap* mountainNoiseMap;
            SpotNoiseMap* structureNoiseMap;
            
            
            void generateChunksAroundPoint(ChunkCoordinates coordinates);
            void draw();
            void drawChunk(OverworldChunk* chunk);
            void addGenerationPatternsForChunk(std::vector<PatternGenerationObject>& patterns, ChunkCoordinates position, const TerrainBiome& biome);
            void addGenerationPattern(std::vector<PatternGenerationObject>& patterns, float terrainHeightValue, float treeGenerationValue, float structureGenerationValue, OverworldPosition position, const TerrainBiome& biome);
            OverworldChunk* generateChunk(ChunkCoordinates position);
            int generateTile(float terrainHeightValue, const TerrainBiome& biome);
            void applyPattern(int tiles[OVERWORLD_CHUNK_SIZE][OVERWORLD_CHUNK_SIZE], std::vector<OverworldObject>& objects, PatternGenerationObject& pattern, ChunkCoordinates& chunkPosition, const TerrainBiome& biome);
            OverworldObject generateOverworldObject(const TerrainBiome& biome, ObjectType type, OverworldPosition position);

        public:
            void update();
            void reloadChunksAroundPoint(ChunkCoordinates coordinates);
            ~OverworldTerrain();
            OverworldTerrain();


    };

}

#endif