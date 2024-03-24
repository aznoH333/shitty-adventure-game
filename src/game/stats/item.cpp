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
                playerStats->getI(a.first) += a.second.value.intValue;

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



    Item::Item(int itemId, int itemSeed, float targetItemScore){
        bool isEmpty = false;
        this->itemId = itemId;
        int seedCopy = itemSeed;
        std::vector<int> usedIds = std::vector<int>();
        float itemScore = 0.0f;
        float differentialValue = getPseudoRandomFloat(1, 3, seedCopy++);

        {
            // generate upsides
            do {
                generateStat(seedCopy, usedIds, true, itemScore);
            }while (itemScore < targetItemScore + differentialValue);

            // generate downsides
            while (itemScore > targetItemScore){
                generateStat(seedCopy, usedIds, false, itemScore);

            }
        }



        // generate name and sprite
        {
            // name
            name = concatSprite("item id : ", itemId);

            // sprite
            int randomSpriteIndex = getPseudoRandomInt(1, MAX_SPRITE_INDEX, seedCopy++);
            sprite = concatSprite(SPRITE_NAME, randomSpriteIndex);
        }

        // print values to console
        std::cout << "\nItem stats \n";
        for (std::pair<int, StatModifier> p : modifiers){
            std::cout << p.first << " : " << (p.second.type == INT ? p.second.value.intValue : p.second.value.floatValue) << "\n";
        }
        std::cout << "\nfinal item score " << itemScore << "\n";
    }


    Item::Item(int itemId){
        this->itemId = itemId;
        sprite = concatSprite(SPRITE_NAME, EMPTY_SPRITE_INDEX);
        name = "Nothing";
        bool isEmpty = true;
    }

    

    void Item::generateStat(int& seed, std::vector<int>& usedIds, bool shouldBePositive, float& itemScore){
        int result = playerStats->pickRandomStat(seed++, usedIds);
        usedIds.push_back(result);
        StatType type = playerStats->getStatType(result);

        switch(type){
            case INT:
                addIntStat(result, itemScore, seed, shouldBePositive);
                break;
            case FLOAT:
                addFloatStat(result, itemScore, seed, shouldBePositive);
                break;
        }
    }


    void Item::addIntStat(int id, float& itemScore, int& statSeed, bool isPositive){
        
        Stat<int>& stat = playerStats->getIStat(id);
        float modifier = getPseudoRandomFloat(0.7f, 1.3f, statSeed++);
        int positiveModifier = boolToSign(isPositive);
        int power = stat.getAverageValueChange() * modifier * positiveModifier;
        itemScore += (stat.getWeight() * modifier) * positiveModifier;
        modifiers[id] = {INT, {power}};

    }

    void Item::addFloatStat(int id, float& itemScore, int& statSeed, bool isPositive){
        
        Stat<float>& stat = playerStats->getFStat(id);
        float modifier = getPseudoRandomFloat(0.7f, 1.3f, statSeed++);
        int positiveModifier = boolToSign(isPositive);
        float power = stat.getAverageValueChange() * modifier * positiveModifier;
        itemScore += (stat.getWeight() * modifier) * positiveModifier;
        modifiers[id] = {FLOAT, {.floatValue = power}};
    
    }

    bool Item::isEmpty(){
        return isEmptyItem;
    }

    std::string& Item::getName(){
        return name;
    }
    std::string& Item::getSprite(){
        return sprite;
    }

    

}