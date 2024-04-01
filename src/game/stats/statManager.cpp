#include "game/stats/statManager.h"
#include "engine/utils.h"
#include <vector>



using namespace Utils;
namespace PlayerStats{
    StatManager* StatManager::instance = 0;
    
    StatManager* StatManager::get(){
        if (!instance){
            instance = new StatManager();
        }
        return instance;
    }
        
    StatManager::StatManager(){
        // ints
        intStats.insert({SHELL_RELOAD_TIME_i, Stat<int>(15, LOW, -3, "reload speed")});
        intStats.insert({AMMO_COUNT_i, Stat<int>(4, UNUPGRADABLE, 0, "")});
        intStats.insert({MAX_AMMO_COUNT_i, Stat<int>(4, MED, 2, "max ammo")});
        intStats.insert({FIRING_SPEED_i, Stat<int>(20, HIGH, -4, "fire rate")});
        intStats.insert({PROJECTILE_COUNT_i, Stat<int>(5, HIGH, 2, "projectiles")});

        // floats
        floatStats.insert({PROJECTILE_SPREAD_f, Stat<float>(5.0f, MED, -0.5f, "spread")});
        floatStats.insert({PROJECTILE_SPEED_f, Stat<float>(6.0f, LOW, 3.4f, "bullet speed")});
        floatStats.insert({PROJECTILE_DAMAGE_f, Stat<float>(7.0f, HIGH, 1.0f, "damage")});
        floatStats.insert({HEALTH_f, Stat<float>(100.0f, UNUPGRADABLE, 0.0f, "")});
        floatStats.insert({MAX_HEALTH_f, Stat<float>(100.0f, MED, 20.0f, "health")});
    }

    void StatManager::resetAll(){
        for(auto a : intStats){
            a.second.resetStat();
        }

        for(auto a : floatStats){
            a.second.resetStat();
        }
    }

    int& StatManager::getI(int index){
        return intStats.at(index).get();
    }

    float& StatManager::getF(int index){
        return floatStats.at(index).get();
    }

    int StatManager::pickRandomStat(int randomBase, std::vector<int>& excludeStats){
        return pickRandomStatWithWeightLessThan(randomBase, HIGHEST, excludeStats);
    }

    int StatManager::pickRandomStatWithWeightLessThan(int randomBase, int weight, std::vector<int>& excludeStats){
        std::vector<int> possibleOutputs = std::vector<int>();

        for (auto a : intStats){
            if (a.second.getWeight() <= weight && !isInVec<int>(excludeStats, a.first)){
                possibleOutputs.push_back(a.first);
            }
        }

        for (auto a : floatStats){
            if (a.second.getWeight() <= weight && !isInVec<int>(excludeStats, a.first)){
                possibleOutputs.push_back(a.first);
            }
        }

        int random = getPseudoRandomInt(0, possibleOutputs.size() - 1, randomBase);

        return possibleOutputs[random];
    }

    StatType StatManager::getStatType(int statIndex){
        if (statIndex < intStats.size()){
            return INT;
        }else {
            return FLOAT;
        }
    }

    Stat<int>& StatManager::getIStat(int statId){
        return intStats.at(statId);
    }

    Stat<float>& StatManager::getFStat(int statId){
        return floatStats.at(statId);
    }

}