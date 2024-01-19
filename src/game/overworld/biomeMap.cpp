#include "game/overworld/biomeNoiseMap.h"
#include <iostream>

namespace TerrainGeneration{
    
    void BiomeNoiseMap::generateNoiseValue(ChunkCoordinates position){
        if (noise.find(position) == noise.end()){
            OverworldPosition spotPosition = {Utils::getPseudoRandomInt(0, noiseResolution, noiseSeed) + (position.x * noiseResolution), Utils::getPseudoRandomInt(0, noiseResolution, noiseSeed)  + (position.y * noiseResolution)};
            
            noise[position] = {spotPosition, Utils::getPseudoRandomInt(0, 1, Utils::hashVector(position.x, position.y) + noiseSeed)};
        }
    }
    
    int BiomeNoiseMap::getBiome(OverworldPosition& position){
        
        // shitty workaround for weird division stuff
        int noiseX = Utils::dividePosition(position.x, noiseResolution);
        int noiseY = Utils::dividePosition(position.y, noiseResolution);
        std::cout << noiseX << ", " << noiseY << " | " << position.x << ", " << position.y << "\n";

        bool isOutSet = false;
        float outputDist = 0;
        BiomeSpot* output;
        for (int i = noiseX - 1; i <= noiseX + 1; i++){
            for (int j = noiseY - 1; j <= noiseY + 1; j++){
                generateNoiseValue({i, j});

                BiomeSpot& cell = noise[{i, j}];
                float distanceToCell = Utils::pythagoras(position.x, position.y, cell.position.x, cell.position.y);

                if (!isOutSet || distanceToCell < outputDist){
                    output = &cell;
                    outputDist = distanceToCell;
                    isOutSet = true;
                }
            }
        }
        return output->biome;
    }
    
    
    
    
    BiomeNoiseMap::BiomeNoiseMap(int noiseSeed, int biomeResolution){
        this->noiseSeed = noiseSeed;
        this->noiseResolution = biomeResolution;
    }
    BiomeNoiseMap::~BiomeNoiseMap(){

    }
}
            