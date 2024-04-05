#ifndef ENEMY_MANAGER
#define ENEMY_MANAGER

#include <map>
#include <vector>
#include "dungeonEnemy.h"
namespace DungeonCode {
    
    

    class EnemyManager{
        private:
            const int MAX_POWER_LEVEL = 10;
            const float ENEMY_HEALTH_BASELINE = 80.0f;
            const float ENEMY_HEALTH_POWER_MULTIPLIER = 40.0f;
            const float ENEMY_HEALTH_DEVIATION = 0.2;
            const float MIN_MOVE_SPEED = 0.7f;
            const float MAX_MOVE_SPEED = 2.3f;

            
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