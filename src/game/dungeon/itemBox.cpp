#include "game/dungeon/itemBox.h"
#include "engine/drawing.h"
#include "engine/utils.h"

using namespace Utils;
namespace DungeonCode {


    ItemBox::ItemBox(Vector2 position, int item){
        this->position = position;
        this->item = item;
        this->isOpen = false;
    }

    void ItemBox::update(){
        if (!isOpen){
            Drawing::get()->drawTexture("chest_1", position, 0, 1.0f, 0.0f, WHITE, DrawingLayers::LAYER_OBJECT);
        }else {
            itemOffset = gravitateValue(itemOffset, 1.0f, OFFSET_SPEED);
            float offset = smoothstep(itemOffset);

            
            
            Drawing::get()->drawTexture("chest_2", position, 0, 1.0f, 0.0f, WHITE, DrawingLayers::LAYER_OBJECT);
            Drawing::get()->drawTexture("chest_2", {position.x + OFFSET_X, position.y + (offset * OFFSET_IN_PIXELS)}, 0, 1.0f, 0.0f, WHITE, DrawingLayers::LAYER_OBJECT);
        }
    }


    void ItemBox::switchItem(int* otherItem){
        int temp = *otherItem;
        *otherItem = item;
        item = temp;
    }

    

    void ItemBox::openBox(){
        isOpen = true;
        itemOffset = 0;
    }

    bool ItemBox::isBoxOpen(){
        return isOpen;
    }

    Vector2& ItemBox::getPosition(){
        return position;
    }

}