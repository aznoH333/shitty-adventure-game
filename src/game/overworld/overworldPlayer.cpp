#include "game/overworld/overworld.h"



OverworldPlayer::OverworldPlayer(TerrainGeneration::OverworldPosition position){
    this->position = position;
    this->subPosition = {0,0};
}


void OverworldPlayer::update(){
    // temporary shitty update

    
    TerrainGeneration::ChunkCoordinates playerChunk = 
        { Utils::dividePosition(position.x, TerrainGeneration::OVERWORLD_CHUNK_SIZE), 
          Utils::dividePosition(position.y, TerrainGeneration::OVERWORLD_CHUNK_SIZE)};



    Overworld::get()->setPlayerChunk(playerChunk);




    
    if (IsKeyPressed(KEY_W)){
        position.y -= 1;
    }
    if (IsKeyPressed(KEY_S)){
        position.y += 1;
    }
    if (IsKeyPressed(KEY_A)){
        position.x -= 1;
    }
    if (IsKeyPressed(KEY_D)){
        position.x += 1;
    }

    Drawing::get()->getCamera().target = {position.x * OVERWORLD_TILE_SIZE, position.y * OVERWORLD_TILE_SIZE};
    Drawing::get()->drawTexture("placeholders_7", {position.x * OVERWORLD_TILE_SIZE, position.y * OVERWORLD_TILE_SIZE}, false, 1, 0, WHITE, DrawingLayers::LAYER_PLAYER);
}