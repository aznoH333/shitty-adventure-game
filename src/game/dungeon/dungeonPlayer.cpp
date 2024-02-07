#include "game/dungeon/dungeon.h"
#include <iostream>

using namespace DungeonCode;


DungeonPlayer::DungeonPlayer(Vector2 position){
    this->position = position;
    // couldn't figure out why
    // but the player has to start a pixel heigher othervise he is stuck in the floor
    // got introduced with wall climbing
    // not urgent enough to fix
    this->position.y -= 1.0f;
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
    buttonLeft = IsKeyDown(KEY_A);
    buttonRight = IsKeyDown(KEY_D);
    buttonJumpPressed = IsKeyPressed(KEY_SPACE);
    buttonJump = IsKeyDown(KEY_SPACE);
    

    // no button or both buttons pressed
    if (buttonLeft == buttonRight){
        
        walkVal = Utils::gravitateValue(walkVal, 0.0f, WALK_ACCELERATION);

    // move left
    }else if (buttonLeft){

        walkVal = Utils::gravitateValue(walkVal, -1.0f, WALK_ACCELERATION);

    // move right
    }else if (buttonRight){
        
        walkVal = Utils::gravitateValue(walkVal, 1.0f, WALK_ACCELERATION);


    }

    if (jumpBuffer > 0){
        tryJump();
        tryWallClimbJump();
    }else{
        // reset jump buffer
        jumpHeightBuffer *= buttonJump;

    }
}

void DungeonPlayer::tryMove(){

    velocity.x = (walkVal * WALK_SPEED) + additionalHorizontalVelocity;

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

// --== jumping and climbing ==--
void DungeonPlayer::tryJump(){
    if (isAirborne == false){
        velocity.y = -JUMP_FORCE;
        jumpHeightBuffer = JUMP_HEIGHT_BUFFER_LENGTH;
    }
}

void DungeonPlayer::tryWallClimbJump(){
    if (canWallClimb){
        
        float wallClimbStrength = std::max(1.0f - (wallClimbCounter * WALL_CLIMB_DECAY), 0.0f);
        
        if (wallClimbStrength <= 0.1f){
            // dont climb if strength too weak
            return;
        }

        velocity.y = -WALL_CLIMB_JUMP_FORCE * wallClimbStrength;
        // no jump buffer for wall climbs
        additionalHorizontalVelocity = (buttonLeft * 2 - 1) * WALL_CLIMB_PUSH_FORCE * wallClimbStrength;


        wallClimbCounter++;

    }

}


void DungeonPlayer::updateMovementValues(){
    isAirborne = !Dungeon::get()->advancedDungeonCollisions({position.x, position.y + SIZE.y + 1.0f}, {SIZE.x, 1.0f}, position, platformPtr);

    if (isAirborne || (platformPtr != nullptr && velocity.y < -0.1f)){
        // in air
        velocity.y += GRAVITY * (jumpHeightBuffer == 0);
    }else if (platformPtr != nullptr){
        // on platform
        velocity.y = platformPtr->speed;
        position.y = platformPtr->position.y - SIZE.y;
        platformPtr->isFalling = !platformPtr->isWaterFall;
    }else {
        // on ground
        velocity.y = 0.0f;
        wallClimbCounter = 0;
    }

    jumpHeightBuffer -= jumpHeightBuffer > 0;


    // wall climbing
    // rules
    // has to be air borne
    // and near a wall
    // and holding key towards wall
    // and falling (vel > 0)
    canWallClimb = isAirborne 
        && Dungeon::get()->collidesWithDungeon({position.x  - 1.0f, position.y}, {SIZE.x + 2.0f, SIZE.y}, false) 
        && (buttonLeft != buttonRight) 
        && velocity.y > 0.0f;
    

    // jump buffer
    if (buttonJumpPressed){
        jumpBuffer = JUMP_BUFFER_LENGTH;
    }
    jumpBuffer -= jumpBuffer > 0;

    additionalHorizontalVelocity = Utils::gravitateValue(additionalHorizontalVelocity, 0.0f, ADDITIONAL_VELOCITY_DECAY_SPEED);
}