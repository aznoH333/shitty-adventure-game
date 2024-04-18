#ifndef DUNGEON_ENEMY
#define DUNGEON_ENEMY

#include "engine/utils.h"
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
        int contactDamage;
        bool hasRangedAttack;
        int rangedAttackCooldown;
        int rangedDamage;
        float size;
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
            int contactDamage = 0;
            Color color;
            bool hasRangedAttack;
            int rangedAttackCooldownMax;
            int rangedDamage;
            int rangedAttackCooldown;
            float size;
            Utils::GenericTimer hitTimer = Utils::GenericTimer(20);
            const float HIT_SIZE_TIMER_MULTIPLIER = 0.4f;
            Utils::GenericTimer shootSizeTimer = Utils::GenericTimer(20);
            const float SHOOT_SIZE_TIMER_MULTIPLIER = 0.2f;

            
        
        public:
            static constexpr Vector2 ENEMY_SIZE = {16.0f, 32.0f};
            float health = 0;
            float lastHitDirection = 0.0f;
            const float CHANCE_TO_SHOOT = 0.1f;


            DungeonEnemy(Vector2 position, int behaviourType, int attackType, int spriteBase, float moveSpeed, float health);
            DungeonEnemy(Vector2 position, EnemyInitializer initializer);
            std::string getSprite();
            Vector2& getPosition();
            Vector2 getAttackPosition();
            int getAttackSpriteIndex();
            bool isAttacking();
            bool& getFlip();
            Color& getColor();
            int& getSpr();
            float getMovementSpeed();
            void update();
            void dispose();
            int getContactDamage();
            void tryShooting();
            float getSize();
            void takeDamage(float damage);

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
        "spider_1_",
        "spider_2_",
        "squid_1_",
        "squid_2_",
    };

    const std::vector<std::string> enemyAttackSprites = {
        "bullet_1",
    };
}


#endif
