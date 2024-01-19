#include "game/overworld/valueNoise.h"

#include <iostream>
namespace TerrainGeneration {


    void ValueNoiseMap::generateNoiseValue(OverworldPosition position){
        if (noise.find(position) == noise.end()){
            noise[position] = Utils::getPseudoRandomFloat(Utils::hashVector(position.x, position.y) + Utils::hashInt(noiseSeed));
        }
    }

    float ValueNoiseMap::getNoiseValue(OverworldPosition& position, int resolution){
        

        int noiseX = Utils::dividePosition(position.x, resolution);
        int noiseY = Utils::dividePosition(position.y, resolution);

        





        for (int i = noiseX - 2; i < noiseX + 2; i++){
            for (int j = noiseY - 2; j < noiseY + 2; j++){
                generateNoiseValue({i, j});
            }
        }
        
        float topLeftValue = noise[{noiseX, noiseY}];
        float topRightValue = noise[{noiseX + 1, noiseY}];
        float bottomLeftValue = noise[{noiseX, noiseY + 1}];
        float bottomRightValue = noise[{noiseX + 1, noiseY + 1}];



        float xInterp = Utils::smoothstep((float) (std::abs(position.x + (position.x < 0)) % resolution) / resolution);
        float yInterp = Utils::smoothstep((float) (std::abs(position.y + (position.y < 0)) % resolution) / resolution);

        if (position.x < 0){
            xInterp = 1.0f - xInterp;
        }

        if (position.y < 0){
            yInterp = 1.0f - yInterp;
        }

        float a = Utils::interpolate(topLeftValue, topRightValue,  xInterp);
        float b = Utils::interpolate(bottomLeftValue, bottomRightValue,  xInterp);
        
        return Utils::interpolate(a, b,  yInterp);
        
    }



    ValueNoiseMap::ValueNoiseMap(int noiseSeed){
        this->noiseSeed = noiseSeed;
    }

    ValueNoiseMap::~ValueNoiseMap(){
    }

}
