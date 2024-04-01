#include "game/stats/statManager.h"
#include "engine/utils.h"
#include "game/stats/statVariable.h"
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
        stats.insert({SHELL_RELOAD_TIME, new Stat(10, 19, 30.0f, 1.5f, "reload speed")});
        stats.insert({AMMO_COUNT, new StatVariable(4, "ammo")});
        stats.insert({MAX_AMMO_COUNT, new Stat(4, 12, 1.0f, "max ammo")});
        stats.insert({FIRING_SPEED, new Stat(10, 19, 40.0f, 2.0f, "fire rate")});
        stats.insert({PROJECTILE_COUNT, new Stat(5, 15, 1.0f, "projectiles")});

        stats.insert({PROJECTILE_SPREAD, new Stat(10, 19, 40.0f, 3.5, "spread")});
        stats.insert({PROJECTILE_SPEED, new Stat(5, 10, 1.0f, "bullet speed")});
        stats.insert({PROJECTILE_DAMAGE, new Stat(10, 19, 0.7, "damage")});
        stats.insert({HEALTH, new StatVariable(100, "health")});
        stats.insert({MAX_HEALTH, new Stat(5, 20, 20.0f, "max health")});
    }

    StatManager::~StatManager(){
        for (auto a : stats){
            delete a.second;
        }
    }

    void StatManager::resetAll(){
        for (auto a : stats){
            a.second->resetStat();
        }
    }

    Stat& StatManager::getStat(int statId){
        return *stats.at(statId);
    }

    int StatManager::get(int statId){
        return stats.at(statId)->get();
    }

    float StatManager::getF(int statId){
        return stats.at(statId)->getF();
    }

    int StatManager::pickRandomStat(int randomBase, std::vector<int>& excludeStats){
        return pickRandomStatWithWeightLessThan(randomBase, excludeStats);
    }

    int StatManager::pickRandomStatWithWeightLessThan(int randomBase, std::vector<int>& excludeStats){
        std::vector<int> possibleOutputs = std::vector<int>();

        for (auto a : stats){
            if (a.second->isUpgradable() && !isInVec(excludeStats, a.first)){
                possibleOutputs.push_back(a.first);
            }
        }

        int random = getPseudoRandomInt(0, possibleOutputs.size() - 1, randomBase);

        return possibleOutputs[random];
    }

}