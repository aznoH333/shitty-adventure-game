#include "game/overworldTerrain.h"





void SpotNoiseMap::generateNoiseValue(ChunkCoordinates position){
    if (noise.find(position) == noise.end()){
        OverworldPosition spotPosition = {generator->getRandomInt(0, OVERWORLD_CHUNK_SIZE) + (position.x * OVERWORLD_CHUNK_SIZE), generator->getRandomInt(0, OVERWORLD_CHUNK_SIZE)  + (position.y * OVERWORLD_CHUNK_SIZE)};
        
        
        
        noise[position] = {spotPosition, generator->getRandomFloat()};
    }
}



float SpotNoiseMap::getNoiseValue(OverworldPosition& position, float minNoiseRadius, float maxNoiseRadius){
    
    float e = maxNoiseRadius - minNoiseRadius;
    
    int noiseX = position.x / OVERWORLD_CHUNK_SIZE;
    int noiseY = position.y / OVERWORLD_CHUNK_SIZE;

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
    generator = new Utils::SeededGenerator(noiseSeed);
}

SpotNoiseMap::~SpotNoiseMap(){
    delete generator;
}