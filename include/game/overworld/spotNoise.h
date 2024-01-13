#ifndef SPOT_NOISE_MAP
#define SPOT_NOISE_MAP


#include "terrainStructs.h"
#include <map>
#include "engine/utils.h"

namespace TerrainGeneration {


    struct NoiseSpot {
        OverworldPosition position;
        float radius;
    };


    class SpotNoiseMap{
        private:
            std::map<ChunkCoordinates, NoiseSpot> noise; 
            int noiseSeed;
            void generateNoiseValue(ChunkCoordinates position);
        public:
            float getNoiseValue(OverworldPosition& position, float minNoiseRadius, float maxNoiseRadius, int resolution);
            SpotNoiseMap(int noiseSeed);
            ~SpotNoiseMap();
    };

}

#endif