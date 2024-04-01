#include "game/stats/itemManager.h"

namespace PlayerStats {
    ItemManager* ItemManager::instance = 0;

    ItemManager::ItemManager(){
        // generate empty items
        for (int i = 0; i < ITEM_CAPACITY; i++){
            equipedItems.push_back(0);
        }
        
        // insert empty item
        items.insert({0, Item(0)});
        items.insert({1, Item(1,120, 5)});

        //equipItemIntoSlot(0, 1);
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
    
    int ItemManager::getItemInSlot(int slotId){
        return equipedItems.at(slotId);
    }

    void ItemManager::equipItemIntoSlot(int slotId, int itemId){
        items.at(equipedItems[slotId]).unequipItem();
        equipedItems[slotId] = itemId;
        items.at(itemId).equipItem();
    }
    
    void ItemManager::dispose(){
        delete instance;
    }

    int ItemManager::getCurrentlySelectedSlot(){
        return currentlySelectedSlot;
    }
    void ItemManager::incrementSelectedSlot(){
        currentlySelectedSlot++;
        if (currentlySelectedSlot > ITEM_CAPACITY){
            currentlySelectedSlot = 0;
        }
    }
}