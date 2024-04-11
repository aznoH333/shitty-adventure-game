#ifndef SLIME_AI
#define SLIME_AI

#include "../enemyAi.h"
#include "../dungeonEnemy.h"

namespace EnemyAiCode {
    class SlimeAi : public DungeonCode::EnemyAi{
        private:
            Vector2 velocity = {0.0f, 0.0f};
            int jumpCooldown = 50;
        public:
            void update(DungeonCode::DungeonEnemy* self);
    };
}

#endif