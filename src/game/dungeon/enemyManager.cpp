#include "game/dungeon/enemyManager.h"
#include "engine/utils.h"
#include "game/dungeon/enemyAi.h"
#include "raymath.h"
#include <iostream>

using namespace Utils;

namespace DungeonCode{
    EnemyManager* EnemyManager::instance = 0;

    EnemyManager* EnemyManager::get(){
        if (!instance) instance = new EnemyManager();
        return instance;
    }

    EnemyManager::EnemyManager(){
        //init(69);
    }

    void EnemyManager::init(int seed){
        // initialize enemies
        for (int i = 0; i < MAX_POWER_LEVEL; i++){
            enemyInitializerMap.insert({i, std::vector<EnemyInitializer>()});
            for (int j = 0; j < getPseudoRandomInt(2, 3, seed++); j++){
                generateEnemy(i, seed);
                seed++;
            }
        }
    }

    void EnemyManager::generateEnemy(int powerLevel, int& seed){
        EnemyInitializer initializer = {};
        float weight = getPseudoRandomFloat(1.0f - ENEMY_WEIGHT_DEVIATION, 1.0f + ENEMY_WEIGHT_DEVIATION, seed++);
        initializer.aiId = getPseudoRandomInt(0, EnemyAiTypes::ENEMY_AI_COUNT - 1, seed++);
        initializer.attackType = 0; // TODO attack types
        initializer.health = (ENEMY_HEALTH_BASELINE + (powerLevel * ENEMY_HEALTH_POWER_MULTIPLIER)) * weight;
        initializer.moveSpeed = getPseudoRandomFloat(MIN_MOVE_SPEED, MAX_MOVE_SPEED, seed++) / weight;
        initializer.sprieBase = getPseudoRandomInt(0, enemySprites.size() - 1, seed++);
        initializer.contactDamage = getPseudoRandomInt(MIN_CONTACT_DAMAGE, MAX_CONTACT_DAMAGE, seed++) * (powerLevel * DAMAGE_MULTIPLIER);
        
        // lvl percentage
        float lvlPercentage = std::min(((float)(powerLevel - 1) / (MAX_POWER_LEVEL - 1)), 1.0f);
        // color
        initializer.color = {
            (unsigned char) interpolate(LOW_LEVEL_COLOR.r, HIGH_LEVEL_COLOR.r, lvlPercentage),
            (unsigned char) interpolate(LOW_LEVEL_COLOR.g, HIGH_LEVEL_COLOR.g, lvlPercentage),
            (unsigned char) interpolate(LOW_LEVEL_COLOR.b, HIGH_LEVEL_COLOR.b, lvlPercentage),
            255
        };

        initializer.size = interpolate(LOW_LEVEL_SIZE, HIGH_LEVEL_SIZE, lvlPercentage);
        
        // ranged attack
        initializer.hasRangedAttack = getPseudoRandomFloat(seed++) <= PROBABILITY_OF_RANGED_ATTACK;
        initializer.rangedDamage = getPseudoRandomInt(MIN_RANGED_DAMAGE, MAX_RANGED_DAMAGE, seed++) * (powerLevel * DAMAGE_MULTIPLIER);
        initializer.rangedAttackCooldown = getPseudoRandomInt(MIN_RANGED_COOLDOWN, MAX_RANGED_COOLDOWN, seed++);



        enemyInitializerMap.at(powerLevel).push_back(initializer);
    }

    DungeonEnemy EnemyManager::initEnemy(Vector2 position, int powerLevel, int& seed){
        // generate new enemies if necesary
        if (enemyInitializerMap.find(powerLevel) == enemyInitializerMap.end()){
            for (int i = 0; i < 3; i++){
                generateEnemy(powerLevel, seed);
            }
        }
        
        
        std::vector<EnemyInitializer>& vec = enemyInitializerMap.at(powerLevel);
        EnemyInitializer& initializer = vec.at(getPseudoRandomInt(0, vec.size() - 1, seed++));
        return DungeonEnemy(position, initializer);
    }


    void EnemyManager::dispose(){
        delete instance;
    }
}