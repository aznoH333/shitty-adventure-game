#include "game/dungeon/dungeon.h"
#include <iostream>

using namespace DungeonCode;


DungeonPlayer::DungeonPlayer(Vector2 position){
    this->position = position;
}

void DungeonPlayer::update(){
    
    readPlayerInput();
    
    // set platform ptr
    platformPtr = nullptr;
    
    tryMove();
    updateMovementValues();
    

    Dungeon::get()->setCameraPosition(position);
    Drawing::get()->drawTexture("dungeon_test_2", position, false, 1, 0, WHITE, DrawingLayers::LAYER_PLAYER);
}


// --== Movement ==--

void DungeonPlayer::readPlayerInput(){
    
    if (IsKeyDown(KEY_A) == IsKeyDown(KEY_D)){
        
        walkVal = Utils::gravitateValue(walkVal, 0.0f, WALK_ACCELERATION);

    }else if (IsKeyDown(KEY_A)){

        walkVal = Utils::gravitateValue(walkVal, -1.0f, WALK_ACCELERATION);

        
    }else if (IsKeyDown(KEY_D)){
        
        walkVal = Utils::gravitateValue(walkVal, 1.0f, WALK_ACCELERATION);


    }

    if (IsKeyDown(KEY_SPACE)){
        tryJump();
    }
}

void DungeonPlayer::tryMove(){

    velocity.x = walkVal * WALK_SPEED;

    if (!Dungeon::get()->collidesWithDungeon({position.x + velocity.x, position.y}, SIZE, false)){
        position.x += velocity.x;
    }else {
        //position.x = position.x - std::fmod(position.x, DUNGEON_TILE_SIZE) + (Utils::relu(velocity.x) * DUNGEON_TILE_SIZE);
        velocity.x = 0.0f;
        walkVal = 0.0f;
    }

    if (!Dungeon::get()->collidesWithDungeon({position.x, position.y + velocity.y}, SIZE, false)){
        position.y += velocity.y;
    }else {
        // snap position to nearest tile
        //position.y = position.y - std::fmod(position.y, DUNGEON_TILE_SIZE) + (Utils::relu(velocity.y) * DUNGEON_TILE_SIZE);
        velocity.y = 0.0f;
        
    }

}

void DungeonPlayer::tryJump(){
    //if (isAirborne == false){
        velocity.y = -JUMP_FORCE;
    //}
}


void DungeonPlayer::updateMovementValues(){

    isAirborne = !Dungeon::get()->advancedDungeonCollisions({position.x, position.y + SIZE.y + 1.0f}, {SIZE.x, 1.0f}, position, platformPtr);

   
    
    
    if (isAirborne || (platformPtr != nullptr && velocity.y < -0.1f)){
        // in air
        velocity.y += GRAVITY;
    }else if (platformPtr != nullptr){
        // on platform
        velocity.y = platformPtr->speed;
        position.y = platformPtr->position.y - SIZE.y;
        platformPtr->isFalling = !platformPtr->isWaterFall;
    }else {
        // on ground
        velocity.y = 0.0f;
    }


    
}