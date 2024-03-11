#include "game/stats/item.h"
#include "game/stats/playerStats.h"


namespace PlayerStats {

    void Item::equipItem(){
        for (std::pair<int, StatModifier> a : modifiers){
            if (a.second.type == INT){
                playerStats->getI(a.first) += a.second.value.intValue;
            }else{
                playerStats->getF(a.first) += a.second.value.floatValue;
            }
        }
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
        // generate some garbage
    }

}