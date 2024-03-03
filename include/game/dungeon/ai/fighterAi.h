#ifndef FIGHTER_AI
#define FIGHTER_AI

#include "../enemyAi.h"
#include "../dungeonEnemy.h"

namespace EnemyAiCode {
    class FighterAi : public DungeonCode::EnemyAi{
        private:
        
        public:
            void update(DungeonCode::DungeonEnemy* self);
    };
}

#endif