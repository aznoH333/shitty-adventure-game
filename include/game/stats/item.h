#ifndef ITEM 
#define ITEM 

#include <map>
#include <vector>
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
            void generateStat(int& seed, std::vector<int>& usedIds, bool shouldBePositive, float& itemScore);


        public:
            Item(int itemId, int itemSeed, float targetItemScore);
            void equipItem();
            void unequipItem();
            bool isEmpty();
    };
}


#endif