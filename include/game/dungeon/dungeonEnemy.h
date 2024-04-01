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
            bool isActive = false;
            float moveSpeed = 0;

            
        
        public:
            static constexpr Vector2 ENEMY_SIZE = {16.0f, 32.0f};
            float health = 0;
            float lastHitDirection = 0.0f;


            DungeonEnemy(Vector2 position, int behaviourType, int attackType, int spriteBase, float moveSpeed, float health);
            std::string getSprite();
            Vector2& getPosition();
            bool& getFlip();
            int& getSpr();
            float getMovementSpeed();
            void update();
            void dispose();

    };


    const std::vector<std::string> enemySprites = {
        "testEnemy_",
    };
}


#endif
