#ifndef BIOME_MAP
#define BIOME_MAP 


#include "terrainStructs.h"
#include <map>
#include "engine/utils.h"

namespace TerrainGeneration {


    struct BiomeSpot {
        OverworldPosition position;
        int biome;
    };


    class BiomeNoiseMap{
        private:
            std::map<ChunkCoordinates, BiomeSpot> noise; 
            int noiseSeed;
            int noiseResolution;
            void generateNoiseValue(ChunkCoordinates position);
        public:
            int getBiome(OverworldPosition& position);
            BiomeNoiseMap(int noiseSeed, int biomeResolution);
            ~BiomeNoiseMap();
    };

}



#endif