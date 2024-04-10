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
        //delete ai;
    }

    Color& DungeonEnemy::getColor(){
        return color;
    }

    int DungeonEnemy::getContactDamage(){
        return contactDamage;
    }

}