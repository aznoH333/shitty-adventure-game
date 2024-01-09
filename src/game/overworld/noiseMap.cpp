#include "game/overworldTerrain.h"





void NoiseMap::generateNoiseValue(OverworldPosition position){
    if (noise.find(position) == noise.end()){
        noise[position] = generator->getRandomFloat();
    }
}

float NoiseMap::getNoiseValue(OverworldPosition& position){
    

    
    int noiseX = position.x / noiseResolution;
    int noiseY = position.y / noiseResolution;



    for (int i = noiseX - 2; i < noiseX + 2; i++){
        for (int j = noiseY - 2; j < noiseY + 2; j++){
            generateNoiseValue({i, j});
        }
    }
    
    float topLeftValue = noise[{noiseX, noiseY}];
    float topRightValue = noise[{noiseX + 1, noiseY}];
    float bottomLeftValue = noise[{noiseX, noiseY + 1}];
    float bottomRightValue = noise[{noiseX + 1, noiseY + 1}];

    float a = Utils::interpolate(topLeftValue, topRightValue,  (float) (position.x % noiseResolution) / noiseResolution);
    float b = Utils::interpolate(bottomLeftValue, bottomRightValue,  (float) (position.x % noiseResolution) / noiseResolution);


    
    return Utils::interpolate(a, b,  (float) (position.y % noiseResolution) / noiseResolution);
    
}

NoiseMap::NoiseMap(int noiseSeed, int noiseResolution){
    this->noiseSeed = noiseSeed;
    this->noiseResolution = noiseResolution;
    generator = new Utils::SeededGenerator(noiseSeed);
}

NoiseMap::~NoiseMap(){
    delete generator;
}