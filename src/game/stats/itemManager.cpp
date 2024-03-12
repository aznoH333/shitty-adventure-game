#include "game/stats/itemManager.h"

namespace PlayerStats {
    ItemManager* ItemManager::instance = 0;

    ItemManager::ItemManager(){
        items.insert({0, Item(0,69)});

        items.at(0).equipItem();
    }


    ItemManager* ItemManager::get(){
        if (!instance){
            instance = new ItemManager();
        }
        return instance;
    }
    
    
    void ItemManager::dispose(){
        delete instance;
    }
}