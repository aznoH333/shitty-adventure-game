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
        stats.insert({SHELL_RELOAD_TIME, (new Stat(30, -2.0f, "reload speed"))->declareMaxValue(10)});
        stats.insert({AMMO_COUNT, new StatVariable(4, "ammo")});
        stats.insert({MAX_AMMO_COUNT, new Stat(4, 1.0f, "max ammo")});
        stats.insert({FIRING_SPEED, (new Stat(30, -2.0f, "firing speed"))->declareMaxValue(10)});
        stats.insert({PROJECTILE_COUNT, new Stat(5, 1.0f, "projectiles")});
        stats.insert({PROJECTILE_SPREAD, (new Stat(20, -1.5f, "accuracy"))->declareMaxValue(10)});
        stats.insert({PROJECTILE_SPEED, new Stat(7, 1.0f, "range")});
        stats.insert({PROJECTILE_DAMAGE, new Stat(8, 0.7f, "damage")});
        stats.insert({HEALTH, new StatVariable(100, "h")});
        stats.insert({MAX_HEALTH, (new Stat(100, 25.0f, "health"))->disableUpgrading()});
        stats.insert({HEALING_POTION_COUNT, new StatVariable(1, "p")});
        stats.insert({HEALING_POTION_MAX, (new Stat(1, 1.0f, "potions"))->disableUpgrading()});

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