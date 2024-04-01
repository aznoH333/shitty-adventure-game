#include "game/dungeon/itemBox.h"
#include "engine/drawing.h"
#include "engine/utils.h"

using namespace Utils;
using namespace PlayerStats;
namespace DungeonCode {


    ItemBox::ItemBox(Vector2 position, int item){
        this->position = position;
        this->itemId = item;
        this->isOpen = false;
        this->item = ItemManager::get()->getItem(item);
    }

    void ItemBox::update(){
        if (!isOpen){
            Drawing::get()->drawTexture("chest_1", position, 0, 1.0f, 0.0f, WHITE, DrawingLayers::LAYER_OBJECT);
        }else {
            itemOffset = gravitateValue(itemOffset, 1.0f, OFFSET_SPEED);
            float offset = smoothstep(itemOffset) + sin(gameTickCounter, ITEM_HOVER_SPEED, ITEM_HOVER_OFFSET);

            
            
            Drawing::get()->drawTexture("chest_2", position, 0, 1.0f, 0.0f, WHITE, DrawingLayers::LAYER_OBJECT);
            if (item->isEmpty() == false){
                Drawing::get()->drawTexture(item->getSprite(), {position.x + OFFSET_X, position.y - (offset * OFFSET_IN_PIXELS)}, 0, 1.0f, 0.0f, WHITE, DrawingLayers::LAYER_OBJECT);
            }
        }
    }


    void ItemBox::setItemId(int itemId){
        this->itemId = itemId;
        this->item = ItemManager::get()->getItem(itemId);
    }

    

    void ItemBox::openBox(){
        isOpen = true;
        itemOffset = 0;
    }

    bool ItemBox::isBoxOpen(){
        if (item->isEmpty()) return false;
        return isOpen;
    }

    Vector2& ItemBox::getPosition(){
        return position;
    }

    int ItemBox::getItemId(){
        return itemId;
    }

}