#ifndef DUNGEON_ENEMY
#define DUNGEON_ENEMY

#include "raylib.h"
#include <string>
#include <vector>

namespace DungeonCode {
    class EnemyAi;
    
    class DungeonEnemy{
        private:

            Vector2 position;
            int sprite = 0;
            int spriteBase = 0;
            bool flipSprite = false;
            EnemyAi* ai;
            int health = 100;
            bool isActive = false;
            float moveSpeed = 0;
        
        public:
            static constexpr Vector2 ENEMY_SIZE = {16.0f, 16.0f};

            DungeonEnemy(Vector2 position, int behaviourType, int attackType, int spriteBase, float moveSpeed);
            std::string getSprite();
            Vector2& getPosition();
            bool& getFlip();
            int& getSpr();
            int& getHealth();
            float getMovementSpeed();
            void update();

    };


    const std::vector<std::string> enemySprites = {
        "dungeon_test_",
    };
}


#endif
