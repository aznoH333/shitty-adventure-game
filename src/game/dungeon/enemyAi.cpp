#include "game/dungeon/enemyAi.h"
#include "game/dungeon//ai/fighterAi.h"

namespace DungeonCode {
    using namespace EnemyAiCode;
    void EnemyAi::update(DungeonEnemy* self){}

    EnemyAi* getEnemyAi(int aiType){
        return new FighterAi();
    }
}