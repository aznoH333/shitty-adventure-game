#ifndef ENEMY_MANAGER
#define ENEMY_MANAGER

#include <map>
#include <vector>
#include "dungeonEnemy.h"
namespace DungeonCode {
    
    

    class EnemyManager{
        private:
            const int MAX_POWER_LEVEL = 10;
            const float ENEMY_HEALTH_BASELINE = 40.0f;
            const float ENEMY_HEALTH_POWER_MULTIPLIER = 25.0f;
            const float ENEMY_WEIGHT_DEVIATION = 0.2;
            const float MIN_MOVE_SPEED = 0.5f;
            const float MAX_MOVE_SPEED = 1.5f;
            const int MIN_COLOR = 180;
            const int MAX_COLOR = 255;
            const float DAMAGE_MULTIPLIER = 1.2f;
            const int MIN_CONTACT_DAMAGE = 10;
            const int MAX_CONTACT_DAMAGE = 22;
            const float PROBABILITY_OF_RANGED_ATTACK = 0.4f;
            const int MIN_RANGED_COOLDOWN = 60;
            const int MAX_RANGED_COOLDOWN = 240;
            const int MIN_RANGED_DAMAGE = 5;
            const int MAX_RANGED_DAMAGE = 25;

            
            static EnemyManager* instance;
            std::map<int, std::vector<EnemyInitializer>> enemyInitializerMap;



        public:
            static EnemyManager* get();
            EnemyManager();
            void init(int seed);
            void generateEnemy(int powerLevel, int& seed);
            DungeonEnemy initEnemy(Vector2 position, int powerLevel, int& seed);
            void dispose();
    };
}


#endif