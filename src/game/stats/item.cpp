#include "game/stats/item.h"
#include "game/stats/playerStats.h"
#include "engine/utils.h"
#include <vector>
#include <iostream>


using namespace Utils;
namespace PlayerStats {

    void Item::equipItem(){
        std::cout << "equiping item " << itemId << "\n\n";
        for (std::pair<int, StatModifier> a : modifiers){
            if (a.second.type == INT){
                std::cout << "added " << a.second.value.intValue << " to stat with id " << a.first <<"\n";
            }else{
                std::cout << "added " << a.second.value.floatValue << " to stat with id " << a.first <<"\n";
                playerStats->getF(a.first) += a.second.value.floatValue;
            }
        }
        std::cout << "done \n\n";
    }

    void Item::unequipItem(){
        for (std::pair<int, StatModifier> a : modifiers){
            if (a.second.type == INT){
                playerStats->getI(a.first) -= a.second.value.intValue;
            }else{
                playerStats->getF(a.first) -= a.second.value.floatValue;
            }
        }
    }

    Item::Item(int itemId, int itemSeed){
        this->itemId = itemId;
        int seedCopy = itemSeed;
        {
            std::vector<int> usedIds = std::vector<int>();
            // generate upsides
            float itemScore = 0.0f;
            int upsideCount = getPseudoRandomInt(1, 2, seedCopy++);
            for (int i = 0; i < upsideCount; i++){
                int result = playerStats->pickRandomStat(seedCopy++, usedIds);
                usedIds.push_back(result);
                StatType type = playerStats->getStatType(result);

                switch(type){
                    case INT:
                        addIntStat(result, itemScore, itemSeed, true);
                        break;
                    case FLOAT:
                        addFloatStat(result, itemScore, itemSeed, true);
                        break;
                }
            }

            // generate downsides
            while(itemScore > 0.0f){
                switch(type){
                    case INT:
                        addIntStat(result, itemScore, itemSeed, true);
                        break;
                    case FLOAT:
                        addFloatStat(result, itemScore, itemSeed, true);
                        break;
                }
            }
        }
    }

    void Item::addIntStat(int id, float& itemScore, int& statSeed, bool isPositive){
        
        Stat<int>& stat = playerStats->getIStat(id);
        float modifier = getPseudoRandomFloat(0.7f, 1.3f, statSeed++) * 200.0f;
        int positiveModifier = boolToSign(isPositive);
        int power = stat.getAverageValueChange() * modifier * positiveModifier;
        itemScore += (stat.getWeight() * modifier) * positiveModifier;
        modifiers[id] = {INT, {power}};

    }

    void Item::addFloatStat(int id, float& itemScore, int& statSeed, bool isPositive){
        
        Stat<float>& stat = playerStats->getFStat(id);
        float modifier = getPseudoRandomFloat(0.7f, 1.3f, statSeed++) * 200.0f;
        int positiveModifier = boolToSign(isPositive);
        float power = stat.getAverageValueChange() * modifier * positiveModifier;
        itemScore += (stat.getWeight() * modifier) * positiveModifier;
        modifiers[id] = {FLOAT, {.floatValue = power}};
    
    }

}