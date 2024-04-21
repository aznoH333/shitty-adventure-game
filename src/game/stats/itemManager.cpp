#include "game/stats/itemManager.h"
#include "engine/utils.h"
#include "game/stats/permanentUpgrade.h"

using namespace Utils;

namespace PlayerStats {
    ItemManager* ItemManager::instance = 0;

    ItemManager::ItemManager(){
        // generate empty items
        for (int i = 0; i < ITEM_CAPACITY; i++){
            equipedItems.push_back(0);
        }
        
        // insert empty item
        items.insert({0, new Item(0)});


        //equipItemIntoSlot(0, 1);
    }


    ItemManager* ItemManager::get(){
        if (!instance){
            instance = new ItemManager();
        }
        return instance;
    }

    Item* ItemManager::getItem(int itemId){
        return items.at(itemId);
    }
    
    int ItemManager::getItemInSlot(int slotId){
        return equipedItems.at(slotId);
    }

    void ItemManager::equipItemIntoSlot(int slotId, int itemId){
        items.at(equipedItems[slotId])->unequipItem();
        equipedItems[slotId] = itemId;
        items.at(itemId)->equipItem();
    }
    
    void ItemManager::consumeItem(int itemId){
        items.at(itemId)->equipItem();
    }

    void ItemManager::dispose(){
        for (std::pair<int, Item*> i : items){
            delete i.second;
        }
        items.clear();
        delete instance;
    }

    int ItemManager::getCurrentlySelectedSlot(){
        return currentlySelectedSlot;
    }
    void ItemManager::incrementSelectedSlot(){
        currentlySelectedSlot++;
        if (currentlySelectedSlot >= ITEM_CAPACITY){
            currentlySelectedSlot = 0;
        }
    }

    int ItemManager::generateNewItem(int itemSeed, float targetItemScore){
        Item* generatedItem;

        if (getPseudoRandomFloat(itemSeed++) <= 0.6f){
            generatedItem = new Item(nextItemId, itemSeed, targetItemScore);
        }else {
            generatedItem = new PermanentItem(nextItemId, itemSeed);
        }
        
        items.insert({nextItemId, generatedItem});
        int i = nextItemId;
        nextItemId++;
        return i;
    }
}