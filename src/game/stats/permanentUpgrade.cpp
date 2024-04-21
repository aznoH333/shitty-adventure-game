#include "game/stats/permanentUpgrade.h"
#include "engine/utils.h"
#include <iostream>

using namespace Utils;

namespace PlayerStats{

    PermanentItem::PermanentItem(int itemId, int itemSeed) : Item(itemId){
        this->isEmptyItem = false;
        this->itemId = itemId;
        int seedCopy = itemSeed;
        StatName possibleStats[] = {MAX_HEALTH, HEALING_POTION_MAX};
        StatName targetStat = possibleStats[getPseudoRandomInt(0, 1, seedCopy++)];

        // add stat
        {
            addStat(targetStat, 1, seedCopy, true);
        }

        // generate name and sprite
        {
            int spriteIndex;
            // name
            switch (targetStat){
                default:
                case MAX_HEALTH:
                    name = "health rune";
                    spriteIndex = 14;
                    break;
                case HEALING_POTION_MAX:
                    name = "empty flask";
                    spriteIndex = 1;
                    break;
            }

            // sprite
            sprite = concatSprite(SPRITE_NAME, spriteIndex);
        }

        equipable = false;
    }

};