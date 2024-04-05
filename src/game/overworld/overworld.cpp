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
    player = new OverworldPlayer({6,0});
    terrain = new TerrainGeneration::OverworldTerrain();
}

void Overworld::init(int seed){
    delete player;
    player = new OverworldPlayer({0, 0});
    terrain->init(seed);
    
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
    terrain->chunkLoadingUpdate();
    if (terrain->isDoneLoading()){
        terrain->update();
        player->update();
    }else {
        UICode::Text::renderGameText("loading world...", {0, 0}, WHITE, 1.0f);
    }
    // debug draw main chunk
    /*
    Drawing::get()->drawTexture("placeholders_7", 
    {playerChunk.x * TerrainGeneration::OVERWORLD_CHUNK_SIZE * OVERWORLD_TILE_SIZE, playerChunk.y * TerrainGeneration::OVERWORLD_CHUNK_SIZE * OVERWORLD_TILE_SIZE}
    , false, 1, 0, WHITE, DrawingLayers::LAYER_PLAYER);*/
}


bool Overworld::collidesWithTerrain(Vector2 position, Vector2 size){

    for (int x = (int)std::floor(position.x); x <= (int)std::ceil(position.x + size.x); x++){
        for (int y = (int)std::floor(position.y); y <= (int)std::ceil(position.y + size.y); y++){
    
    
            int tileX = Utils::dividePosition((int) x, OVERWORLD_TILE_SIZE); 
            int tileY = Utils::dividePosition((int) y, OVERWORLD_TILE_SIZE);

            TerrainGeneration::ChunkCoordinates coords = {
                Utils::dividePosition(tileX, TerrainGeneration::OVERWORLD_CHUNK_SIZE),
                Utils::dividePosition(tileY, TerrainGeneration::OVERWORLD_CHUNK_SIZE)
            };
            // 🤮
            tileX = (tileX < 0) ? 
                (TerrainGeneration::OVERWORLD_CHUNK_SIZE - 1) - (std::abs(tileX + 1) % TerrainGeneration::OVERWORLD_CHUNK_SIZE) : 
                tileX % TerrainGeneration::OVERWORLD_CHUNK_SIZE;
            
            tileY = (tileY < 0) ? 
                (TerrainGeneration::OVERWORLD_CHUNK_SIZE - 1) - (std::abs(tileY + 1) % TerrainGeneration::OVERWORLD_CHUNK_SIZE) : 
                tileY % TerrainGeneration::OVERWORLD_CHUNK_SIZE;

            



            if (terrain->isTileBlocking(coords, tileX, tileY)){
                return true;
            }

        }
    }
    return false;
}

std::vector<TerrainGeneration::OverworldObject*> Overworld::getNearbyObjects(TerrainGeneration::OverworldPosition position, float radius){
    return terrain->getNearbyObjects(position, radius);
}

Vector2 overworldPosToVec(TerrainGeneration::OverworldPosition position){
    return {position.x * OVERWORLD_TILE_SIZE, position.y * OVERWORLD_TILE_SIZE};
}

TerrainGeneration::OverworldTerrain* Overworld::getTerrain(){
    return terrain;
}
