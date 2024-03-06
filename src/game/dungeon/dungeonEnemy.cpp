#include "game/dungeon/dungeonEnemy.h"
#include <string>
#include "game/dungeon/enemyAi.h"

namespace DungeonCode {
    DungeonEnemy::DungeonEnemy(Vector2 position, int behaviourType, int attackType, int sprieBase, float moveSpeed){
        this->position = position;
        this->position.y -= 1.0f; // unstick enemies from ground
        this->spriteBase = sprieBase;
        this->sprite = 3;
        this->ai = getEnemyAi(behaviourType);
        this->moveSpeed = moveSpeed;
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

    int& DungeonEnemy::getHealth(){
        return health;
    }

    float DungeonEnemy::getMovementSpeed(){
        return moveSpeed;
    }
}