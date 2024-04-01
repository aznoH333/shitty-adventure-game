#include "game/stats/item.h"
#include "game/stats/playerStats.h"
#include "engine/utils.h"
#include <vector>
#include <iostream>


using namespace Utils;
namespace PlayerStats {

    void Item::equipItem(){
        for (std::pair<int, int> a : modifiers){
            playerStats->getStat(a.first).addToValue(a.second);
        }
    }

    void Item::unequipItem(){
        for (std::pair<int, int> a : modifiers){
            playerStats->getStat(a.first).addToValue(-a.second);
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
    }


    Item::Item(int itemId){
        this->itemId = itemId;
        sprite = concatSprite(SPRITE_NAME, EMPTY_SPRITE_INDEX);
        name = "nothing";
        this->isEmptyItem = true;
    }

    

    void Item::generateStat(int& seed, std::vector<int>& usedIds, bool shouldBePositive, float& itemScore){
        int result = playerStats->pickRandomStat(seed++, usedIds);
        usedIds.push_back(result);
        addStat(result, itemScore, seed, shouldBePositive);
    }

    void Item::addStat(int id, float& itemScore, int& statSeed, bool isPositive){
        Stat& stat = playerStats->getStat(id);
        float modifier = getPseudoRandomInt(1, 2, statSeed++);
        int positiveModifier = boolToSign(isPositive);
        int power = modifier * positiveModifier;
        itemScore += power;
        modifiers[id] = power;
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

    std::map<int, int>& Item::getModifierList(){
        return modifiers;
    }

    

}