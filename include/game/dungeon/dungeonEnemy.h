#ifndef DUNGEON_ENEMY
#define DUNGEON_ENEMY

#include "raylib.h"
#include <string>
#include <vector>

namespace DungeonCode {
    class EnemyAi;

    struct EnemyInitializer{
        int aiId;
        int attackType; 
        int sprieBase; 
        float moveSpeed; 
        float health;
        Color color;
    };
    
    class DungeonEnemy{
        private:

            Vector2 position;
            int sprite = 0;
            int spriteBase = 0;
            bool flipSprite = false;
            EnemyAi* ai;
            bool isActive = false;
            float moveSpeed = 0;
            Color color;

            
        
        public:
            static constexpr Vector2 ENEMY_SIZE = {16.0f, 32.0f};
            float health = 0;
            float lastHitDirection = 0.0f;


            DungeonEnemy(Vector2 position, int behaviourType, int attackType, int spriteBase, float moveSpeed, float health);
            DungeonEnemy(Vector2 position, EnemyInitializer initializer);
            std::string getSprite();
            Vector2& getPosition();
            bool& getFlip();
            Color& getColor();
            int& getSpr();
            float getMovementSpeed();
            void update();
            void dispose();

    };


    const std::vector<std::string> enemySprites = {
        "skeleton_1_",
        "skeleton_2_",
        "skeleton_3_",
        "skeleton_4_",
        "skeleton_5_",
        "slime_1_",
        "slime_2_",
        "slime_3_",



    };
}


#endif
