#include "game/overworldTerrain.h"


OverworldTerrain::OverworldTerrain(){
    
    // dumbass tried to generate the chunks before the noise
    noiseMap = new NoiseMap(1020, 2);
    
    // init some garbage data
    for (int i = 0; i < LOADED_CHUNK_COUNT; i++){
        loadedChunks[i] = generateChunk({i % 3,i/3});
    }

}

OverworldTerrain::~OverworldTerrain(){
    // free loaded chunks
    for (int i = 0; i < LOADED_CHUNK_COUNT; i++){
        delete loadedChunks[i];
    }

    delete noiseMap;
}



// --== Update ==--
void OverworldTerrain::update(){

    draw();
}




// --== Generation ==--
OverworldChunk* OverworldTerrain::generateChunk(ChunkCoordinates position){
    // generate some garbage data
    OverworldChunk* output = new OverworldChunk();
    output->coordinates = position;
    output->worldObjects = {};

    for (int x = 0; x < OVERWORLD_CHUNK_SIZE; x++){
        for (int y = 0; y < OVERWORLD_CHUNK_SIZE; y++){
            output->tiles[x][y] = generateTile({position.x * OVERWORLD_CHUNK_SIZE + x, position.y * OVERWORLD_CHUNK_SIZE + y});
        }
    }

    // generate some garbage objects
    for (int i = 0; i < 3; i++){
        output->worldObjects.push_back(OverworldObject{"placeholders_6", {position.x * OVERWORLD_CHUNK_SIZE + GetRandomValue(0, OVERWORLD_CHUNK_SIZE), position.y * OVERWORLD_CHUNK_SIZE + GetRandomValue(0, OVERWORLD_CHUNK_SIZE)}});
    }


    return output;

}

int OverworldTerrain::generateTile(OverworldPosition position){
    if (noiseMap->getNoiseValue(position) > 0.5){
        return 0;
    }
    return 2;

}



// --== Drawing ==--
void OverworldTerrain::draw(){
    for (int i = 0; i < LOADED_CHUNK_COUNT; i++){
        drawChunk(loadedChunks[i]);
    }
}

void OverworldTerrain::drawChunk(OverworldChunk* chunk){
    // draw tiles
    for (int x = 0; x < OVERWORLD_CHUNK_SIZE; x++){
        for (int y = 0; y < OVERWORLD_CHUNK_SIZE; y++){
            const OverworldTile tile = tileLookupTable[chunk->tiles[x][y]];
            Vector2 position = {((chunk->coordinates.x * OVERWORLD_CHUNK_SIZE) + x) * OVERWORLD_TILE_SIZE, ((chunk->coordinates.y * OVERWORLD_CHUNK_SIZE) + y) * OVERWORLD_TILE_SIZE};
            Drawing::get()->drawTexture(tile.sprite, position, false, 1, 0, WHITE, DrawingLayers::LAYER_WORLD);
        
        }
    }

    // draw objects
    for (OverworldObject object : chunk->worldObjects){
        Drawing::get()->drawTexture(object.sprite, {object.position.x * OVERWORLD_TILE_SIZE, object.position.y * OVERWORLD_TILE_SIZE}, false, 1, 0, WHITE, LAYER_OBJECT);
    }
}