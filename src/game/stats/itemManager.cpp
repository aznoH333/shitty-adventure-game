#include "game/stats/itemManager.h"

namespace PlayerStats {
    ItemManager* ItemManager::instance = 0;

    ItemManager::ItemManager(){
        items.insert({0, Item(0,120, 5)});

        items.at(0).equipItem();
    }


    ItemManager* ItemManager::get(){
        if (!instance){
            instance = new ItemManager();
        }
        return instance;
    }

    Item* ItemManager::getItem(int itemId){
        return &items.at(itemId);
    }
    
    
    void ItemManager::dispose(){
        delete instance;
    }
}