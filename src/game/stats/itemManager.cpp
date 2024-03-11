#include "game/stats/itemManager.h"

namespace PlayerStats {
    ItemManager* ItemManager::instance = 0;

    ItemManager::ItemManager(){
        
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