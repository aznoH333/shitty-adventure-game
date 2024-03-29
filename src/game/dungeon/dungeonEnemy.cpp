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

    std::string DungeonEnemy::getSprite(){
        return enemySprites[spriteBase] + std::to_string(sprite);
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
}