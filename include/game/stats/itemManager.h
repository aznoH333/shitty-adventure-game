#ifndef ITEM_MANAGER
#define ITEM_MANAGER 

#include "item.h"
#include <map>
#include <vector>

namespace PlayerStats {
    
    
    class ItemManager{
        private:
            static ItemManager* instance;
            std::map<int, Item> items;
            std::vector<int> equipedItems;
            const int ITEM_CAPACITY = 5;


        public:
            static ItemManager* get();
            ItemManager();
            void equitItem(int itemId);
            void unequipItem(int itemId);
            Item* getItem(int itemId);
            void dispose();
            int getItemInSlot(int slotId);
            void equipItemIntoSlot(int slotId, int itemId);
    };
}
#endif