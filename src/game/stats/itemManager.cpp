#include "game/stats/itemManager.h"

namespace PlayerStats {

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