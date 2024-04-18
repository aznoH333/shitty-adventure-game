#include "game/overworld/overworld.h"
#include "game/dungeon/dungeon.h"


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


    // display key prompt
    if (isDungeonNearby){
        UICode::Boxes::displayDefaultTextPrompt("press e to enter");

        if (IsKeyPressed(KEY_E)){
            DungeonCode::Dungeon::get()->enterDungeon(nearbyDungeon, Utils::pythagoras(position.x, position.y, 0, 0));
        }
    }

    
    isMoving = false;
    if (IsKeyDown(KEY_W)){
        tryMove({0.0f, -MOVE_SPEED});
        direction = DIRECTION_UP;
    }
    if (IsKeyDown(KEY_S)){
        tryMove({0.0f, +MOVE_SPEED});
        direction = DIRECTION_DOWN;
    }
    if (IsKeyDown(KEY_A)){
        tryMove({-MOVE_SPEED, 0.0f});
        direction = DIRECTION_LEFT;
    }
    if (IsKeyDown(KEY_D)){
        tryMove({MOVE_SPEED, 0.0f});
        direction = DIRECTION_RIGHT;
    }


    draw();
}


void OverworldPlayer::movedToAnotherTile(){
    std::vector<TerrainGeneration::OverworldObject*> nearbyObjects =  Overworld::get()->getNearbyObjects(position, OBJECT_REACH);

    // look for dungeon
    isDungeonNearby = false;
    nearbyDungeon = nullptr;

    for (TerrainGeneration::OverworldObject* object : nearbyObjects){
        if (object->type == TerrainGeneration::OBJECT_DUNGEON){
            nearbyDungeon = object;
            isDungeonNearby = true;
            break;
        }
    }
}

void OverworldPlayer::draw(){
    // camera target
    Drawing::get()->getCamera().target ={position.x * OVERWORLD_TILE_SIZE + (subPosition.x * OVERWORLD_TILE_SIZE), position.y * OVERWORLD_TILE_SIZE + (subPosition.y * OVERWORLD_TILE_SIZE)};
    
    
    int flip = 0;
    std::string sprite;

    switch (direction) {
        case DIRECTION_DOWN:
            sprite = "player_overworld_";
            break;
        case DIRECTION_LEFT:
            sprite = "player_overworld_side_";
            flip = 1;
            break;
        case DIRECTION_RIGHT:
            sprite = "player_overworld_side_";
            flip = 0;
            break;
        case DIRECTION_UP:
            sprite = "player_overworld_backwards_";
            break;
    }

    int walkSprite = 1;

    if (isMoving){
        walkSprite = Utils::animationTimer(1, 4, 7);
        walkSprite = getWalkSprite(walkSprite);
    }

    sprite = Utils::concatSprite(sprite.c_str(), walkSprite);

    Drawing::get()->drawTexture(sprite, {position.x * OVERWORLD_TILE_SIZE + (subPosition.x * OVERWORLD_TILE_SIZE), position.y * OVERWORLD_TILE_SIZE + (subPosition.y * OVERWORLD_TILE_SIZE)}, flip, 1, 0, WHITE, DrawingLayers::LAYER_PLAYER);

}


int OverworldPlayer::getWalkSprite(int input){
    switch (input) {
        case 1:
        case 3:
            return 1;
        case 2:
            return 2;
        case 4:
            return 3;
    }
    return 0;
}


// --== Movement ==--
void OverworldPlayer::tryMove(Vector2 moveBy){
    
    Overworld* overworld = Overworld::get();
    Vector2 currentPos = {(position.x * OVERWORLD_TILE_SIZE) + (subPosition.x * OVERWORLD_TILE_SIZE),(position.y * OVERWORLD_TILE_SIZE) + (subPosition.y * OVERWORLD_TILE_SIZE)};
    int xDir = (moveBy.x > 0) * 2 - 1;
    int yDir = (moveBy.y > 0) * 2 - 1;

    if (!overworld->collidesWithTerrain({currentPos.x + moveBy.x + xDir, currentPos.y}, {OVERWORLD_TILE_SIZE, OVERWORLD_TILE_SIZE})){
        subPosition.x += moveBy.x;
        isMoving = true;
        if (std::abs(subPosition.x) > 0.5f){
            position.x += xDir;
            subPosition.x -= xDir;
            movedToAnotherTile();
        }
    }else {
        //subPosition.x = 0.0f;
    }

    currentPos = {(position.x * OVERWORLD_TILE_SIZE) + (subPosition.x * OVERWORLD_TILE_SIZE),(position.y * OVERWORLD_TILE_SIZE) + (subPosition.y * OVERWORLD_TILE_SIZE)};

    if (!overworld->collidesWithTerrain({currentPos.x, currentPos.y + moveBy.y + yDir}, {OVERWORLD_TILE_SIZE, OVERWORLD_TILE_SIZE})){
        subPosition.y += moveBy.y;

        if (std::abs(subPosition.y) > 0.5f){
            position.y += yDir;
            subPosition.y -= yDir;
            movedToAnotherTile();
        }
    }else {
        //subPosition.y = 0;
    }


    
    
}