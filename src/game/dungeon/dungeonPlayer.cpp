#include "game/dungeon/dungeon.h"
#include "game/dungeon/dungeonUtils.h"
#include <string>

using namespace DungeonCode;
using namespace DungeonUtils;
using namespace PlayerStats;
using namespace Utils;


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
    float absVelocityX = std::abs(velocity.x);
    
    if (isAirborne){
        if (canWallClimb){
            sprite = "player_dungeon_wall_jump";
        }else {
            sprite = "player_dungeon_jump";
        }
    }else if (absVelocityX > 0.5f){

        int dynamicAnimationDuration = WALK_ANIMATION_FRAME_DURATION + (int)(((1.0f - (absVelocityX / WALK_SPEED)) * WALK_ANIMATION_FRAME_DURATION));
        UICode::Text::renderGameText(std::to_string(dynamicAnimationDuration), {0,0}, WHITE, 1.0f);
        int spriteNum = animationTimer(WALK_ANIMATION_START_INDEX, WALK_ANIMATION_END_INDEX, dynamicAnimationDuration);
        bool playAnimationBackwards = velocity.x < 0.5f == !flipSprite;
        spriteNum = (!playAnimationBackwards * 2 - 1) * spriteNum + (playAnimationBackwards * (WALK_ANIMATION_END_INDEX + 1));
        sprite = concatSprite("player_dungeon_walk_", spriteNum);
    
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
    buttonReloadPressed = IsKeyPressed(KEY_R);
    

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

    universalDungeonCollidingUpdate(position, velocity, SIZE, false);
    if (velocity.x == 0){
        walkVal = 0.0f; // left over jank from previous implementation
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
        

        float loadPercentage = ((float) (playerStats.firingSpeed - gunState.shotLoadTimer) / playerStats.firingSpeed);
        float flipRotation = flipSprite * 2.0f - 1.0f;
        float loadOffset = loadPercentage * 360.0f * flipRotation;
        float recoilOffset = ((float) (RECOIL_TIME - (RECOIL_TIME - gunState.recoilTimer)) / RECOIL_TIME) * RECOIL_OFFSET * flipRotation;
        float reloadPrepOffset = ((RELOAD_PREP_MAX - (float)(RELOAD_PREP_MAX - gunState.reloadPrepTimer)) / RELOAD_PREP_MAX) * RELOAD_PREP_OFFSET * flipRotation;
        float reloadInsertOffset = ((playerStats.shellReloadTime - (float)(playerStats.shellReloadTime - gunState.reloadTimer)) / playerStats.shellReloadTime) * RELOAD_INSERT_OFFSET * flipRotation;
        gunState.direction = std::atan2(gunState.position.y - mousePosition.y, gunState.position.x - mousePosition.x) * RAD2DEG - 180 + loadOffset + recoilOffset + reloadPrepOffset + reloadInsertOffset;
        flipSprite = position.x > mousePosition.x;


        // spawn shotgun shell
        if (gunState.shouldSpawnShell == true && loadPercentage > 0.25f){
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

    // reloading
    {
        if (buttonReloadPressed && gunState.currentState == IDLE && playerStats.currentAmmoCount != playerStats.ammoCapacity){
            gunState.currentState = RELOADING;
            gunState.reloadTimer = playerStats.shellReloadTime;
        }

        if (gunState.currentState == RELOADING){
            gunState.reloadPrepTimer += gunState.reloadPrepTimer < RELOAD_PREP_MAX;
        }else{
            gunState.reloadPrepTimer -= gunState.reloadPrepTimer > 0;
        }
        
        
        if (gunState.currentState == RELOADING && gunState.reloadPrepTimer == RELOAD_PREP_MAX){
            gunState.reloadTimer--;
            if (gunState.reloadTimer == 0){
                playerStats.currentAmmoCount++;
                gunState.playRackSoundEffect = true;
                Audio::get()->playSound("shellLoad");

                if (playerStats.currentAmmoCount == playerStats.ammoCapacity){
                    gunState.currentState = IDLE;
                }else {
                    gunState.reloadTimer = playerStats.shellReloadTime;
                }
            }else if (buttonFirePressed){
                gunState.reloadTimer = 0;
                gunState.currentState = IDLE;
            }
        }
    }
    


    // shooting logic
    {
        if (buttonFirePressed && gunState.shotLoadTimer == 0 && playerStats.currentAmmoCount > 0 && gunState.currentState == IDLE && gunState.reloadPrepTimer == 0){
            gunState.currentState = FIRING;
            // shoot
            Dungeon* d = Dungeon::get();
            Vector2 bulletOrigin = gunState.position;
            bulletOrigin.x += std::cos(gunState.direction * DEG2RAD) * BULLET_SPAWN_OFFSET;
            bulletOrigin.y += std::sin(gunState.direction * DEG2RAD) * BULLET_SPAWN_OFFSET;

            Audio::get()->playSound("gun");
            Drawing::get()->addScreenShaker(5);

            playerStats.currentAmmoCount--;


            for (int i = 0; i < playerStats.projectileCount; i++){
                Projectile p;
                p.position = bulletOrigin;
                p.rotation = gunState.direction + Utils::getRandomFloat(-playerStats.projectileSpread, playerStats.projectileSpread);
                p.velocity.x = std::cos(p.rotation * DEG2RAD) * playerStats.projectileSpeed;
                p.velocity.y = std::sin(p.rotation * DEG2RAD) * playerStats.projectileSpeed;
                p.sprite = "bullet_1";
                p.alliedWithPlayer = true;
                p.extraUpdates = 3;
                p.damage = 10.0f;
                d->addProjectile(p);
            }


            gunState.shotLoadTimer = playerStats.firingSpeed;
            gunState.recoilTimer = RECOIL_TIME;
            gunState.shouldSpawnShell = true;
        }else if (playerStats.currentAmmoCount == 0){
            // TOOD play click sound
        }

        gunState.recoilTimer -= gunState.recoilTimer > 0;
        gunState.shotLoadTimer -= gunState.recoilTimer == 0 && gunState.shotLoadTimer > 0;
        if (gunState.shotLoadTimer == 0 && gunState.reloadPrepTimer == 0){
            if (gunState.playRackSoundEffect){
                gunState.playRackSoundEffect = false;
                Audio::get()->playSound("shotgunRack");

            }
            
            gunState.currentState = IDLE;
        }
        

    }


}


void DungeonPlayer::drawGun(){
    Drawing::get()->drawTexture("gun", gunState.position, flipSprite * 2, 1.0f, gunState.direction, WHITE, DrawingLayers::LAYER_PLAYER);

}

