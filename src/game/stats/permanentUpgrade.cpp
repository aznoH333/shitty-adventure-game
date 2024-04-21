#include "game/stats/permanentUpgrade.h"
#include "engine/utils.h"
#include <iostream>

using namespace Utils;

namespace PlayerStats{

    PermanentItem::PermanentItem(int itemId, int itemSeed, StatName targetStat) : Item(itemId){
        this->isEmptyItem = false;
        this->itemId = itemId;
        int seedCopy = itemSeed;
        std::cout << "generated permanent upgrade\n";
        // add stat
        {
            addStat(targetStat, 0, seedCopy, true);
        }

        // generate name and sprite
        {
            // name
            switch (targetStat){
                default:
                case MAX_HEALTH:
                    name = "health upgrade";
                    break;
            }

            // sprite
            int randomSpriteIndex = 14;
            sprite = concatSprite(SPRITE_NAME, randomSpriteIndex);
        }

        equipable = false;
    }

};