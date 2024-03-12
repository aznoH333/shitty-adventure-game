#ifndef ITEM 
#define ITEM 

#include <map>
#include "stat.h"
namespace PlayerStats{
    
    union StatValue{
        int intValue;
        float floatValue;
    };

    struct StatModifier{
        StatType type;
        StatValue value;
    };
    
    class Item{
        private:
            std::map<int, StatModifier> modifiers;
            bool isEmptyItem = false;
            int itemId;

            void addIntStat(int id, float& itemScore, int& statSeed, bool positive);
            void addFloatStat(int id, float& itemScore, int& statSeed, bool positive);


        public:
            Item(int itemId, int itemSeed);
            void equipItem();
            void unequipItem();
    };
}


#endif