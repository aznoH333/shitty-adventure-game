#ifndef ENEMY_AI
#define ENEMY_AI

#include "dungeonEnemy.h"
namespace DungeonCode{
    
    
    class EnemyAi{
        private:
        public:
            virtual void update(DungeonEnemy* self);
    };

    enum EnemyAiTypes{
        FIGHTER = 0,
        SLIME,
        ENEMY_AI_COUNT,
    };

    EnemyAi* getEnemyAi(int aiType);
};

#endif