#include "game/overworld/overworld.h"



OverworldPlayer::OverworldPlayer(TerrainGeneration::OverworldPosition position){
    this->position = position;
    this->subPosition = {0,0};
}



// --== Update ==--
void OverworldPlayer::update(){
    // temporary shitty update

    
    TerrainGeneration::ChunkCoordinates playerChunk = 
        { Utils::dividePosition(position.x, TerrainGeneration::OVERWORLD_CHUNK_SIZE), 
          Utils::dividePosition(position.y, TerrainGeneration::OVERWORLD_CHUNK_SIZE)};



    Overworld::get()->setPlayerChunk(playerChunk);




    
    if (IsKeyDown(KEY_W)){
        tryMove({0.0f, -moveSpeed});
    }
    if (IsKeyDown(KEY_S)){
        tryMove({0.0f, +moveSpeed});
    }
    if (IsKeyDown(KEY_A)){
        tryMove({-moveSpeed, 0.0f});
    }
    if (IsKeyDown(KEY_D)){
        tryMove({moveSpeed, 0.0f});
    }

    Drawing::get()->getCamera().target ={position.x * OVERWORLD_TILE_SIZE + (subPosition.x * OVERWORLD_TILE_SIZE), position.y * OVERWORLD_TILE_SIZE + (subPosition.y * OVERWORLD_TILE_SIZE)};
    Drawing::get()->drawTexture("overworld_player", {position.x * OVERWORLD_TILE_SIZE + (subPosition.x * OVERWORLD_TILE_SIZE), position.y * OVERWORLD_TILE_SIZE + (subPosition.y * OVERWORLD_TILE_SIZE)}, false, 1, 0, WHITE, DrawingLayers::LAYER_PLAYER);
    Drawing::get()->drawTexture("overworld_player", {position.x * OVERWORLD_TILE_SIZE, position.y * OVERWORLD_TILE_SIZE}, false, 1, 0, WHITE, DrawingLayers::LAYER_PLAYER);


}

// --== Movement ==--
void OverworldPlayer::tryMove(Vector2 moveBy){
    
    Overworld* overworld = Overworld::get();
    Vector2 currentPos = {(position.x * OVERWORLD_TILE_SIZE) + (subPosition.x * OVERWORLD_TILE_SIZE),(position.y * OVERWORLD_TILE_SIZE) + (subPosition.y * OVERWORLD_TILE_SIZE)};
    int xDir = (moveBy.x > 0) * 2 - 1;
    int yDir = (moveBy.y > 0) * 2 - 1;

    if (!overworld->collidesWithTerrain({currentPos.x + moveBy.x + xDir, currentPos.y}, {OVERWORLD_TILE_SIZE, OVERWORLD_TILE_SIZE})){
        subPosition.x += moveBy.x;

        if (std::abs(subPosition.x) >= 1.0f){
            position.x += xDir;
            subPosition.x -= xDir;
        }
    }else {
        //subPosition.x = 0.0f;
    }

    currentPos = {(position.x * OVERWORLD_TILE_SIZE) + (subPosition.x * OVERWORLD_TILE_SIZE),(position.y * OVERWORLD_TILE_SIZE) + (subPosition.y * OVERWORLD_TILE_SIZE)};

    if (!overworld->collidesWithTerrain({currentPos.x, currentPos.y + moveBy.y + yDir}, {OVERWORLD_TILE_SIZE, OVERWORLD_TILE_SIZE})){
        subPosition.y += moveBy.y;

        if (std::abs(subPosition.y) >= 1.0f){
            position.y += yDir;
            subPosition.y -= yDir;
        }
    }else {
        //subPosition.y = 0;
    }


    
    
}