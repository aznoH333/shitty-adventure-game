#ifndef VALUE_NOISE
#define VALUE_NOISE

#include "terrainStructs.h"
#include <map>
#include "engine/utils.h"

namespace TerrainGeneration {



    class ValueNoiseMap{
        private:
            std::map<OverworldPosition, float> noise;
            int noiseSeed;
            void generateNoiseValue(OverworldPosition position);
        public:
            float getNoiseValue(OverworldPosition& position, int resolution);
            ValueNoiseMap(int noiseSeed);
            ~ValueNoiseMap();
    };

}

#endif