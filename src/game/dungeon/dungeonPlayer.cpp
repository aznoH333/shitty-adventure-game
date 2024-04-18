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
    deathUpdate();
    
    if (isDead){
        return;
    }

    readPlayerInput();
    
    // set platform ptr
    platformPtr = nullptr;
    tryMove();
    updateMovementValues();
    updateBoxInteraction();
    updateGun();
    updateHealing();
    updateSpikeDamage();
    drawGun();
    postHitInvincibility -= postHitInvincibility > 0;

    Dungeon::get()->setCameraPosition(position);
    if (postHitInvincibility % 4 < 2){
        drawSprite();
    }
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
// --== Death ==--
void DungeonPlayer::deathUpdate(){
    if (!isDead){
        return;
    }

    deathTimer.progress();

    if (deathTimer.isReady()){
        died();
    }
}

void DungeonPlayer::died(){
    exit(0); // TODO death animation
}

// --== Movement ==--
void DungeonPlayer::readPlayerInput(){
    buttonLeft = IsKeyDown(KEY_A);
    buttonRight = IsKeyDown(KEY_D);
    buttonJumpPressed = IsKeyPressed(KEY_SPACE);
    buttonJump = IsKeyDown(KEY_SPACE);
    buttonInteractPressed = IsKeyPressed(KEY_E);
    buttonFirePressed = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
    buttonReloadPressed = IsKeyPressed(KEY_R);
    buttonHealPressed = IsKeyPressed(KEY_F);
    

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
// --== healing ==--
void DungeonPlayer::updateHealing(){
    healthTimer.progress();
    if (buttonHealPressed && healthTimer.isReady() && playerStats->get(HEALING_POTION_COUNT) > 0 && playerStats->get(HEALTH) < playerStats->get(MAX_HEALTH)){
        healthTimer.reset();
        playerStats->getStat(HEALTH).set(playerStats->get(MAX_HEALTH));
        playerStats->getStat(HEALING_POTION_COUNT).addToValue(-1);

        // todo animations
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
        velocity.y += (1.0f - ((float) jumpHeightBuffer / JUMP_HEIGHT_BUFFER_LENGTH)) * GRAVITY;
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
    UICode::Boxes::displayDefaultTextPrompt("press e to enter");

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
        
        int firingSpeed = playerStats->get(FIRING_SPEED);
        int reloadTime = playerStats->get(SHELL_RELOAD_TIME);

        float loadPercentage = ((float) (firingSpeed - gunState.shotLoadTimer) / firingSpeed);
        float flipRotation = flipSprite * 2.0f - 1.0f;
        float loadOffset = loadPercentage * 360.0f * flipRotation;
        float recoilOffset = ((float) (RECOIL_TIME - (RECOIL_TIME - gunState.recoilTimer)) / RECOIL_TIME) * RECOIL_OFFSET * flipRotation;
        float reloadPrepOffset = ((RELOAD_PREP_MAX - (float)(RELOAD_PREP_MAX - gunState.reloadPrepTimer)) / RELOAD_PREP_MAX) * RELOAD_PREP_OFFSET * flipRotation;
        float reloadInsertOffset = ((reloadTime - (float)(reloadTime - gunState.reloadTimer)) / reloadTime) * RELOAD_INSERT_OFFSET * flipRotation;
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
        if (buttonReloadPressed && gunState.currentState == IDLE && playerStats->get(AMMO_COUNT) != playerStats->get(MAX_AMMO_COUNT)){
            gunState.currentState = RELOADING;
            gunState.reloadTimer = playerStats->get(SHELL_RELOAD_TIME);
        }

        if (gunState.currentState == RELOADING){
            gunState.reloadPrepTimer += gunState.reloadPrepTimer < RELOAD_PREP_MAX;
        }else{
            gunState.reloadPrepTimer -= gunState.reloadPrepTimer > 0;
        }
        
        if (gunState.currentState == RELOADING && gunState.reloadPrepTimer == RELOAD_PREP_MAX){
            gunState.reloadTimer--;
            if (gunState.reloadTimer == 0){
                playerStats->getStat(AMMO_COUNT).addToValue(1);
                gunState.playRackSoundEffect = true;
                Audio::get()->playSound("shellLoad");

                if (playerStats->get(AMMO_COUNT) == playerStats->get(MAX_AMMO_COUNT)){
                    gunState.currentState = IDLE;
                }else {
                    gunState.reloadTimer = playerStats->get(SHELL_RELOAD_TIME);
                }
            }else if (buttonFirePressed){
                gunState.reloadTimer = 0;
                gunState.currentState = IDLE;
            }
        }
    }
    
    // shooting logic
    {
        if (buttonFirePressed && gunState.shotLoadTimer == 0 && playerStats->get(AMMO_COUNT)> 0 && gunState.currentState == IDLE && gunState.reloadPrepTimer == 0){
            gunState.currentState = FIRING;
            // shoot
            Dungeon* d = Dungeon::get();
            Vector2 bulletOrigin = gunState.position;
            bulletOrigin.x += std::cos(gunState.direction * DEG2RAD) * BULLET_SPAWN_OFFSET;
            bulletOrigin.y += std::sin(gunState.direction * DEG2RAD) * BULLET_SPAWN_OFFSET;

            Audio::get()->playSound("gun");
            Drawing::get()->addScreenShaker(5);

            playerStats->getStat(AMMO_COUNT).addToValue(-1);

            for (int i = 0; i < playerStats->get(PROJECTILE_COUNT); i++){
                Projectile p;
                p.position = bulletOrigin;
                p.rotation = gunState.direction + Utils::getRandomFloat(-playerStats->getF(PROJECTILE_SPREAD), playerStats->getF(PROJECTILE_SPREAD)) * RAD2DEG;
                p.velocity = playerStats->getF(PROJECTILE_SPEED);
                p.deceleration = 0.1f;
                p.sprite = "bullet_1";
                p.alliedWithPlayer = true;
                p.extraUpdates = 2;
                p.damage = playerStats->getF(PROJECTILE_DAMAGE);
                d->addProjectile(p);
            }

            gunState.shotLoadTimer = playerStats->get(FIRING_SPEED);
            gunState.recoilTimer = RECOIL_TIME;
            gunState.shouldSpawnShell = true;
        }else if (buttonFirePressed && playerStats->get(AMMO_COUNT) == 0){
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


void DungeonPlayer::takeDamage(int damage){
    if (postHitInvincibility == 0){
        playerStats->getStat(HEALTH).addToValue(-damage);
        postHitInvincibility = MAX_POST_HIT_INVINCIBILITY;

        // death 
        if (playerStats->get(HEALTH) < 0){
            isDead = true;
            deathTimer.reset();
        }
    }
}

// --== dungeon spikes ==--
void DungeonPlayer::updateSpikeDamage(){
    if (Dungeon::get()->collidesWithSpikes(Vector2{position.x, position.y + SIZE.y}, {SIZE.x, 3.0f})){
        takeDamage((playerStats->get(MAX_HEALTH) / 4));
    }
}


// --== box stuff ==--
void DungeonPlayer::updateBoxInteraction(){
    ItemBox* box = Dungeon::get()->lookForNearbyBoxes(position);

    if (box == nullptr || isAirborne || !box->isBoxOpen()){
        return;
    }

    // display interaction prompt
    UICode::Boxes::displayItemPrompt(ItemManager::get()->getItem(box->getItemId()));

    if (buttonInteractPressed){
        ItemManager* m = ItemManager::get();
        
        int thisId = m->getItemInSlot(m->getCurrentlySelectedSlot());
        m->equipItemIntoSlot(m->getCurrentlySelectedSlot(), box->getItemId());
        box->setItemId(thisId);
        m->incrementSelectedSlot();
    }

}
