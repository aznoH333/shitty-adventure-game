#ifndef FIGHTER_AI
#define FIGHTER_AI

#include "../enemyAi.h"
#include "../dungeonEnemy.h"

namespace EnemyAiCode {
    class FighterAi : public DungeonCode::EnemyAi{
        private:
            Vector2 velocity = {0.0f, 0.0f};
        public:
            void update(DungeonCode::DungeonEnemy* self);
    };
}

#endif