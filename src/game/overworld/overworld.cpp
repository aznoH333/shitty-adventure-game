#include "game/overworld/overworld.h"

// --== boiler plate ==--


Overworld* Overworld::instance = 0;



Overworld* Overworld::get(){
    if (!instance){
        instance = new Overworld();
    }

    return instance;
}

Overworld::Overworld(){
    player = new OverworldPlayer({0,0});
    terrain = new TerrainGeneration::OverworldTerrain();
}

void Overworld::dispose(){
    delete player;
    delete terrain;
    delete instance;
    
}

void Overworld::setPlayerChunk(TerrainGeneration::ChunkCoordinates coordinates){
    if (coordinates == playerChunk){
        return;
    }
    this->playerChunk = coordinates;
    terrain->reloadChunksAroundPoint(playerChunk);
    
}



// --== logic ==--
void Overworld::update(){
    terrain->update();
    // debug draw main chunk
    Drawing::get()->drawTexture("placeholders_7", 
    {playerChunk.x * TerrainGeneration::OVERWORLD_CHUNK_SIZE * OVERWORLD_TILE_SIZE, playerChunk.y * TerrainGeneration::OVERWORLD_CHUNK_SIZE * OVERWORLD_TILE_SIZE}
    , false, 1, 0, WHITE, DrawingLayers::LAYER_PLAYER);
    player->update();
}