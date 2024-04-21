#include "game/stats/item.h"
#include "game/stats/playerStats.h"
#include "engine/utils.h"
#include <vector>
#include <iostream>


using namespace Utils;
namespace PlayerStats {

    void Item::equipItem(){
        for (std::pair<int, int> a : modifiers){
            Stat& s = playerStats->getStat(a.first);
            
            playerStats->getStat(a.first).addToValue(a.second);

            // exceptions
            switch (a.first) {
                case MAX_AMMO_COUNT:
                    playerStats->getStat(AMMO_COUNT).set(s.get());
                    break;
                case MAX_HEALTH:
                    playerStats->getStat(HEALTH).set(s.get());
                    break;
            }
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
        // item power
        int modifierValue = 1 + (targetItemScore * 0.8f);


        {
            int positiveCount = getPseudoRandomInt(1, 2, itemSeed++);
            
            // generate positive
            for (int i = 0; i < positiveCount; i++){
                generateStat(itemSeed, usedIds, true, modifierValue);
            }
            // generate negative
            //generateStat(itemSeed, usedIds, false, modifierValue);

        }
        // generate name and sprite
        {
            // name
            name = "";
            name += ITEM_NAMES_1.at(getPseudoRandomInt(0, ITEM_NAMES_1.size() - 1, seedCopy++));
            name += " ";
            name += ITEM_NAMES_2.at(getPseudoRandomInt(0, ITEM_NAMES_2.size() - 1, seedCopy++));

            // sprite
            int randomSpriteIndex = getPseudoRandomInt(1, MAX_SPRITE_INDEX, seedCopy++);
            sprite = concatSprite(SPRITE_NAME, randomSpriteIndex);
        }

        equipable = true;
    }


    Item::Item(int itemId){
        this->itemId = itemId;
        sprite = concatSprite(SPRITE_NAME, EMPTY_SPRITE_INDEX);
        name = "nothing";
        this->isEmptyItem = true;
    }

    bool Item::isEquipable(){
        return equipable;
    }

    

    void Item::generateStat(int& seed, std::vector<int>& usedIds, bool shouldBePositive, int modifierValue){
        int result = playerStats->pickRandomStat(seed++, usedIds);
        usedIds.push_back(result);
        addStat(result, modifierValue, seed, shouldBePositive);
    }

    void Item::addStat(int id, int modifierValue, int& statSeed, bool isPositive){
        Stat& stat = playerStats->getStat(id);
        float modifier = modifierValue + getPseudoRandomInt(0, 1, statSeed++);
        int positiveModifier = boolToSign(isPositive);
        int power = modifier * positiveModifier;
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