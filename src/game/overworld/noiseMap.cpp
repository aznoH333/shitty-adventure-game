#include "game/overworldTerrain.h"



namespace TerrainGeneration {


    void ValueNoiseMap::generateNoiseValue(OverworldPosition position){
        if (noise.find(position) == noise.end()){
            noise[position] = generator->getRandomFloat();
        }
    }

    float ValueNoiseMap::getNoiseValue(OverworldPosition& position, int resolution){
        

        
        int noiseX = position.x / resolution;
        int noiseY = position.y / resolution;



        for (int i = noiseX - 2; i < noiseX + 2; i++){
            for (int j = noiseY - 2; j < noiseY + 2; j++){
                generateNoiseValue({i, j});
            }
        }
        
        float topLeftValue = noise[{noiseX, noiseY}];
        float topRightValue = noise[{noiseX + 1, noiseY}];
        float bottomLeftValue = noise[{noiseX, noiseY + 1}];
        float bottomRightValue = noise[{noiseX + 1, noiseY + 1}];

        float a = Utils::interpolate(topLeftValue, topRightValue,  Utils::smoothstep((float) (position.x % resolution) / resolution));
        float b = Utils::interpolate(bottomLeftValue, bottomRightValue,  Utils::smoothstep((float) (position.x % resolution) / resolution));


        
        return Utils::interpolate(a, b,  Utils::smoothstep((float) (position.y % resolution) / resolution));
        
    }



    ValueNoiseMap::ValueNoiseMap(int noiseSeed){
        this->noiseSeed = noiseSeed;
        generator = new Utils::SeededGenerator(noiseSeed);
    }

    ValueNoiseMap::~ValueNoiseMap(){
        delete generator;
    }

}
