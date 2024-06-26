#ifndef ITEM 
#define ITEM 

#include <map>
#include <vector>
#include <string>
namespace PlayerStats{
    
    const std::vector<std::string> ITEM_NAMES_1 = {
        "powerfull",
        "funky",
        "smelly",
        "fast",
        "guilded",
        "armored",
        "deadly",
        "mythical",
        "goofy",
        "legendary",
        "poor",
        "strange",
    };

    const std::vector<std::string> ITEM_NAMES_2 = {
        "shells",
        "buck shot",
        "receiver",
        "bullets",
        "pellets",
        "argus premium",
        "plating",
        "medicine",
        "triger",
        "muzzle",
        "iron sights",

    };
    
    class Item{
        protected:
            bool equipable;
            bool isEmptyItem = false;
            int itemId;
            std::string sprite;
            const int MAX_SPRITE_INDEX = 21;
            const int EMPTY_SPRITE_INDEX = 22;
            const char* SPRITE_NAME = "item_icons_";
            std::string name;
            std::map<int, int> modifiers;
            void addStat(int id, int modifierValue, int& statSeed, bool positive);

        
        private:
            void generateStat(int& seed, std::vector<int>& usedIds, bool shouldBePositive, int modifierValue);

        public:
            Item(int itemId, int itemSeed, float targetItemScore);
            Item(int itemId);
            std::string& getName();
            std::string& getSprite();
            std::map<int, int>& getModifierList();
            void equipItem();
            void unequipItem();
            bool isEmpty();
            bool isPositive();
            bool isEquipable();
    };
}


#endif