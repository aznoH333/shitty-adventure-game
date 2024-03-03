#include "game/dungeon/dungeon.h"

using namespace DungeonCode;


DungeonPlayer::DungeonPlayer(Vector2 position){
    this->position = {position.x, position.y + (16.0f - SIZE.y)};
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
    
    updateGun();
    drawGun();

    Dungeon::get()->setCameraPosition(position);
    drawSprite();
}


void DungeonPlayer::drawSprite(){
    std::string sprite;
    
    if (isAirborne){
        if (canWallClimb){
            sprite = "player_dungeon_wall_jump";
        }else {
            sprite = "player_dungeon_jump";
        }
    }else if (std::abs(velocity.x) > 0.5f){
        sprite = "player_dungeon_walk_" + std::to_string(Utils::animationTimer(1, 3, 10));
    }else {
        sprite = "player_dungeon";
    }
    
    
    Drawing::get()->drawTexture(sprite, position, flipSprite, 1, 0, WHITE, DrawingLayers::LAYER_PLAYER);
}


// --== Movement ==--
void DungeonPlayer::readPlayerInput(){
    buttonLeft = IsKeyDown(KEY_A);
    buttonRight = IsKeyDown(KEY_D);
    buttonJumpPressed = IsKeyPressed(KEY_SPACE);
    buttonJump = IsKeyDown(KEY_SPACE);
    buttonInteractPressed = IsKeyPressed(KEY_E);
    buttonFirePressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    

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
    updateGravity();    
    updateWallClimbing();
    updateDoorInteraction();

    // decay additional speed
    additionalHorizontalVelocity = Utils::gravitateValue(additionalHorizontalVelocity, 0.0f, ADDITIONAL_VELOCITY_DECAY_SPEED);
}


void DungeonPlayer::updateGravity(){
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


    // jump buffer
    if (buttonJumpPressed){
        jumpBuffer = JUMP_BUFFER_LENGTH;
    }
    jumpBuffer -= jumpBuffer > 0;
}

void DungeonPlayer::updateWallClimbing(){
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
}

void DungeonPlayer::updateDoorInteraction(){
    if (nearbyDoor == nullptr || isAirborne){
        return;
    }

    // display interaction prompt
    UICode::Boxes::displayPrompt("press e to enter");

    if (buttonInteractPressed){
        Dungeon::get()->enterDoor(nearbyDoor);
    }
}


// --== getters and setters ==--
Vector2& DungeonPlayer::getPosition(){
    return position;
}

void DungeonPlayer::setNearbyDoor(DungeonDoor* door){
    this->nearbyDoor = door;
}



// --== gun ==--
void DungeonPlayer::updateGun(){
    
    
    // drawing and visuals
    {
        gunState.position = {position.x + (flipSprite ? GUN_OFFSET_X_1 : GUN_OFFSET_X_2), position.y + GUN_OFFSET_Y};
        Vector2 mousePosition = Drawing::get()->getInworldMousePosition();
        

        float reloadPercentage = ((float) (TEMPORARY_RELOAD_TIME - gunState.reloadTimer) / TEMPORARY_RELOAD_TIME);
        float flipRotation = flipSprite * 2.0f - 1.0f;
        float reloadOffset = reloadPercentage * 360.0f * flipRotation;
        float recoilOffset = ((float) (RECOIL_TIME - (RECOIL_TIME - gunState.recoilTimer)) / RECOIL_TIME) * RECOIL_OFFSET * flipRotation;
        gunState.direction = std::atan2(gunState.position.y - mousePosition.y, gunState.position.x - mousePosition.x) * RAD2DEG - 180 + reloadOffset + recoilOffset;
        flipSprite = position.x > mousePosition.x;


        // spawn shotgun shell
        if (gunState.shouldSpawnShell == true && reloadPercentage > 0.25f){
            Audio::get()->playSound("gunReload");
            gunState.shouldSpawnShell = false;
            Dungeon::get()->addGiblet(
                {"bullet_2", 
                gunState.position, 
                {SHELL_EJECT_VELOCITY.x * flipRotation, SHELL_EJECT_VELOCITY.y}, 
                Utils::getRandomFloat(-SHELL_ROTATION_SPEED, SHELL_ROTATION_SPEED), 
                0.0f, 
                0});
        }
    }
    


    // shooting logic
    {
        if (buttonFirePressed && gunState.reloadTimer == 0){
            // shoot
            Dungeon* d = Dungeon::get();
            Vector2 bulletOrigin = gunState.position;
            bulletOrigin.x += std::cos(gunState.direction * DEG2RAD) * BULLET_SPAWN_OFFSET;
            bulletOrigin.y += std::sin(gunState.direction * DEG2RAD) * BULLET_SPAWN_OFFSET;

            Audio::get()->playSound("gun");


            for (int i = 0; i < TEMPORARY_PROJECTILE_COUNT; i++){
                Projectile p;
                p.position = bulletOrigin;
                p.rotation = gunState.direction + Utils::getRandomFloat(-TEMPORARY_PROJECTILE_SPREAD, TEMPORARY_PROJECTILE_SPREAD);
                p.velocity.x = std::cos(p.rotation * DEG2RAD) * TEMPORARY_BULLET_SPEED;
                p.velocity.y = std::sin(p.rotation * DEG2RAD) * TEMPORARY_BULLET_SPEED;
                p.sprite = "bullet_1";
                p.alliedWithPlayer = true;
                p.extraUpdates = 3;
                d->addProjectile(p);
            }


            gunState.reloadTimer = TEMPORARY_RELOAD_TIME;
            gunState.recoilTimer = RECOIL_TIME;
            gunState.shouldSpawnShell = true;
        }
        gunState.recoilTimer -= gunState.recoilTimer > 0;
        gunState.reloadTimer -= gunState.recoilTimer == 0 && gunState.reloadTimer > 0;
    }


}


void DungeonPlayer::drawGun(){
    Drawing::get()->drawTexture("gun", gunState.position, flipSprite * 2, 1.0f, gunState.direction, WHITE, DrawingLayers::LAYER_PLAYER);

}

