#include "game/overworld/valueNoise.h"


namespace TerrainGeneration {


    void ValueNoiseMap::generateNoiseValue(OverworldPosition position){
        if (noise.find(position) == noise.end()){
            noise[position] = Utils::getPseudoRandomFloat(Utils::hashVector(position.x, position.y) + Utils::hashInt(noiseSeed));
        }
    }

    float ValueNoiseMap::getNoiseValue(OverworldPosition& position, int resolution){
        
        

        // shitty workaround for weird division stuff
        int negativeX = position.x < 0;
        int negativeY = position.y < 0;        
        int noiseX = (position.x + negativeX) / resolution; 
        int noiseY = (position.y + negativeY) / resolution;
        noiseX -= negativeX;
        noiseY -= negativeY;


        for (int i = noiseX - 2; i < noiseX + 2; i++){
            for (int j = noiseY - 2; j < noiseY + 2; j++){
                generateNoiseValue({i, j});
            }
        }
        
        float topLeftValue = noise[{noiseX, noiseY}];
        float topRightValue = noise[{noiseX + 1, noiseY}];
        float bottomLeftValue = noise[{noiseX, noiseY + 1}];
        float bottomRightValue = noise[{noiseX + 1, noiseY + 1}];



        float xInterp = Utils::smoothstep((float) (std::abs(position.x + negativeX) % resolution) / resolution);
        float yInterp = Utils::smoothstep((float) (std::abs(position.y + negativeY) % resolution) / resolution);

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
