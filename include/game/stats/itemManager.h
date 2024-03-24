#ifndef ITEM_MANAGER
#define ITEM_MANAGER 

#include "item.h"
#include <map>

namespace PlayerStats {
    
    
    class ItemManager{
        private:
            static ItemManager* instance;
            std::map<int, Item> items;


        public:
            static ItemManager* get();
            ItemManager();
            void equitItem(int itemId);
            void unequipItem(int itemId);
            Item* getItem(int itemId);
            void dispose();
    };
}
#endif