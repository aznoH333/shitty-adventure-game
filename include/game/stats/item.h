#ifndef ITEM 
#define ITEM 

#include <map>
namespace PlayerStats{
    
    enum ModifierType{
        INT = 0,
        FLOAT,
    };

    union StatValue{
        int intValue;
        float floatValue;
    };

    struct StatModifier{
        ModifierType type;
        StatValue value;
    };
    
    
    class Item{
        private:
            std::map<int, StatModifier> modifiers;
            bool isEmptyItem = false;
            int itemId;

        public:
            Item(int itemId, int itemSeed);
            void equipItem();
            void unequipItem();
    };
}


#endif