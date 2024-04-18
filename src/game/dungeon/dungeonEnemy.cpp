#include "game/dungeon/dungeonEnemy.h"
#include <string>
#include "game/dungeon/dungeon.h"
#include "game/dungeon/enemyAi.h"

namespace DungeonCode {
    DungeonEnemy::DungeonEnemy(Vector2 position, int behaviourType, int attackType, int sprieBase, float moveSpeed, float health){
        this->position = position;
        this->position.y -= 1.0f + (ENEMY_SIZE.y - DUNGEON_TILE_SIZE); // unstick enemies from ground
        this->spriteBase = sprieBase;
        this->sprite = 3;
        this->ai = getEnemyAi(behaviourType);
        this->moveSpeed = moveSpeed;
        this->health = health;
    }

    DungeonEnemy::DungeonEnemy(Vector2 position, EnemyInitializer initializer){
        this->position = position;
        this->position.y -= 1.0f + (ENEMY_SIZE.y - DUNGEON_TILE_SIZE);
        this->contactDamage = initializer.contactDamage;
        this->spriteBase = initializer.sprieBase;
        this->sprite = 3;
        this->ai = getEnemyAi(initializer.aiId);
        this->moveSpeed = initializer.moveSpeed;
        this->health = initializer.health;
        this->color = initializer.color;
        this->rangedAttackCooldownMax = initializer.rangedAttackCooldown;
        this->rangedAttackCooldown = this->rangedAttackCooldownMax;
        this->hasRangedAttack = initializer.hasRangedAttack;
        this->rangedDamage = initializer.rangedDamage;
        this->size = initializer.size;
    }

    std::string DungeonEnemy::getSprite(){
        return enemySprites[spriteBase] + std::to_string(sprite);
    }

    int& DungeonEnemy::getSpr(){
        return sprite;
    }

    Vector2& DungeonEnemy::getPosition(){
        return position;
    }

    void DungeonEnemy::update(){
        ai->update(this);
        hitTimer.progress();
        shootSizeTimer.progress();
        rangedAttackCooldown -= rangedAttackCooldown > 0;
    }

    bool& DungeonEnemy::getFlip(){
        return flipSprite;
    }

    float DungeonEnemy::getMovementSpeed(){
        return moveSpeed;
    }

    void DungeonEnemy::dispose(){
        // this causes a double free?
        // some memory magic is going on here
        delete ai;
    }

    Color& DungeonEnemy::getColor(){
        return color;
    }

    float DungeonEnemy::getSize(){
        return size + (hitTimer.getAsPercentage() * HIT_SIZE_TIMER_MULTIPLIER) + (shootSizeTimer.getAsPercentage() * SHOOT_SIZE_TIMER_MULTIPLIER);
    }

    int DungeonEnemy::getContactDamage(){
        return contactDamage;
    }

    Vector2 DungeonEnemy::getAttackPosition(){
        return {position.x + 8.0f, position.y + 16.0f};
    }

    int DungeonEnemy::getAttackSpriteIndex(){
        return 0;
    }

    bool DungeonEnemy::isAttacking(){
        return rangedAttackCooldown > (rangedAttackCooldownMax >> 1);
    }

    void DungeonEnemy::tryShooting(){
        if (!hasRangedAttack || rangedAttackCooldown != 0){
            return;
        }

        if (Utils::getRandomFloat() >= CHANCE_TO_SHOOT){
            return;
        }
        // cooldown
        rangedAttackCooldown = rangedAttackCooldownMax;

        Dungeon* d = Dungeon::get();

        // spawn projectile
        
        Projectile p = Projectile(
            "bullet_1",
            getAttackPosition(), 
            Utils::directionTowards(getPosition(), d->getPlayer()->getPosition()) * DEG2RAD, 
            2.0f, 
            0.1f, 
            rangedDamage, 
            false);
        d->addProjectile(p);
        
        shootSizeTimer.reset();
    }

    void DungeonEnemy::takeDamage(float damage){
        this->health -= damage;
        hitTimer.reset();
    }

}