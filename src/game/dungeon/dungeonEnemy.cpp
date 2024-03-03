#include "game/dungeon/dungeonEnemy.h"
#include <string>
#include "game/dungeon/enemyAi.h"

namespace DungeonCode {
    DungeonEnemy::DungeonEnemy(Vector2 position, int behaviourType, int attackType, int sprieBase){
        this->position = position;
        this->spriteBase = sprieBase;
        this->sprite = 3;
        this->ai = getEnemyAi(behaviourType);
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
}