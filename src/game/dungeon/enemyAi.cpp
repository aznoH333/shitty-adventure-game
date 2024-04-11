#include "game/dungeon/enemyAi.h"
#include "game/dungeon/ai/fighterAi.h"
#include "game/dungeon/ai/slimeAi.h"

namespace DungeonCode {
    using namespace EnemyAiCode;
    void EnemyAi::update(DungeonEnemy* self){}

    EnemyAi* getEnemyAi(int aiType){
        
        switch (aiType) {
            
            case EnemyAiTypes::FIGHTER:
                return new FighterAi();
            case EnemyAiTypes::SLIME:
                return new SlimeAi();
            default:
                exit(-1);
        }
        
    }

    int getEnemyAiCount(){
        return 2;
    }
}