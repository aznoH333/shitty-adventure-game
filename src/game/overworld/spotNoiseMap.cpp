#include "game/overworld/spotNoise.h"



namespace TerrainGeneration {



    void SpotNoiseMap::generateNoiseValue(ChunkCoordinates position){
        if (noise.find(position) == noise.end()){
            OverworldPosition spotPosition = {Utils::getPseudoRandomInt(0, OVERWORLD_CHUNK_SIZE, noiseSeed) + (position.x * OVERWORLD_CHUNK_SIZE), Utils::getPseudoRandomInt(0, OVERWORLD_CHUNK_SIZE, noiseSeed)  + (position.y * OVERWORLD_CHUNK_SIZE)};
            
            noise[position] = {spotPosition, Utils::getPseudoRandomFloat(Utils::hashVector(position.x, position.y) + noiseSeed)};
        }
    }



    float SpotNoiseMap::getNoiseValue(OverworldPosition& position, float minNoiseRadius, float maxNoiseRadius, int resolution){
        
        float e = maxNoiseRadius - minNoiseRadius;
        
        int noiseX = position.x / resolution;
        int noiseY = position.y / resolution;

        float value = 0;
        for (int i = noiseX - 1; i <= noiseX + 1; i++){
            for (int j = noiseY - 1; j <= noiseY + 1; j++){
                generateNoiseValue({i, j});

                NoiseSpot& cell = noise[{i, j}];
                float distanceToCell = Utils::pythagoras(position.x, position.y, cell.position.x, cell.position.y);
                float cellValue = 1 - (distanceToCell / (cell.radius * e + minNoiseRadius));
                cellValue = std::max(cellValue, 0.0f);

                if (cellValue > value){
                    value = cellValue;
                }
            }
        }
        return value;
    }



    SpotNoiseMap::SpotNoiseMap(int noiseSeed){
        this->noiseSeed = noiseSeed;
    }

    SpotNoiseMap::~SpotNoiseMap(){
    }

}