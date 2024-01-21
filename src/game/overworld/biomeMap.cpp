#include "game/overworld/biomeNoiseMap.h"

namespace TerrainGeneration{
    
    void BiomeNoiseMap::generateNoiseValue(ChunkCoordinates position){
        if (noise.find(position) == noise.end()){
            OverworldPosition spotPosition = {
                Utils::getPseudoRandomInt(0, noiseResolution, noiseSeed + Utils::hashVector(position.x, position.y)) + (position.x * noiseResolution), 
                Utils::getPseudoRandomInt(0, noiseResolution, noiseSeed + Utils::hashVector(position.y + 1984, position.x)) + (position.y * noiseResolution)};
            noise[position] = {spotPosition, Utils::getPseudoRandomInt(0, 1, Utils::hashVector(spotPosition.x, spotPosition.y))};
        }
    }
    
    int BiomeNoiseMap::getBiome(OverworldPosition& position){
        
        // shitty workaround for weird division stuff
        int noiseX = Utils::dividePosition(position.x, noiseResolution);
        int noiseY = Utils::dividePosition(position.y, noiseResolution);


        float outputDist = 9999999999.0f;
        int output;
        
        for (int i = noiseX - 1; i <= noiseX + 1; i++){
            for (int j = noiseY - 1; j <= noiseY + 1; j++){
                generateNoiseValue({i, j});
                BiomeSpot& cell = noise[{i, j}];
                float distanceToCell = Utils::pythagoras(position.x, position.y, cell.position.x, cell.position.y);

                if (distanceToCell < outputDist){
                    outputDist = distanceToCell;
                    output = cell.biome;
                }
                
            }
        }
        return output;
    }
    
    
    
    
    BiomeNoiseMap::BiomeNoiseMap(int noiseSeed, int biomeResolution){
        this->noiseSeed = noiseSeed;
        this->noiseResolution = biomeResolution;
    }
    BiomeNoiseMap::~BiomeNoiseMap(){

    }
}
            