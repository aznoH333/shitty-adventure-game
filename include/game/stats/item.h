#ifndef ITEM 
#define ITEM 

#include <map>
#include <vector>
#include <string>
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
            const int MAX_SPRITE_INDEX = 21;
            const int EMPTY_SPRITE_INDEX = 22;
            const char* SPRITE_NAME = "item_icons_";
            int itemId;
            std::string sprite;
            std::string name;

            void addIntStat(int id, float& itemScore, int& statSeed, bool positive);
            void addFloatStat(int id, float& itemScore, int& statSeed, bool positive);
            void generateStat(int& seed, std::vector<int>& usedIds, bool shouldBePositive, float& itemScore);


        public:
            Item(int itemId, int itemSeed, float targetItemScore);
            Item(int itemId);
            std::string& getName();
            std::string& getSprite();
            std::map<int, StatModifier>& getModifierList();
            void equipItem();
            void unequipItem();
            bool isEmpty();
    };
}


#endif