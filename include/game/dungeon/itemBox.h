#ifndef ITEM_BOX
#define ITEM_BOX

#include "raylib.h" 
#include "game/stats/itemManager.h"
#include "game/stats/item.h"

namespace DungeonCode {
    class ItemBox{
        private:
            Vector2 position;
            int itemId;
            PlayerStats::Item* item;
            
            bool isOpen = false;
            float itemOffset = 0.0f;
            const float OFFSET_IN_PIXELS = 10.0f;
            const float OFFSET_SPEED = 0.02f;
            const float OFFSET_X = 8.0f; 
            const float ITEM_HOVER_SPEED = 0.05f;
            const float ITEM_HOVER_OFFSET = 1.0f;

        public:
            ItemBox(Vector2 position, int item);
            void update();
            void setItemId(int itemId);
            Vector2& getPosition();
            void openBox();
            bool isBoxOpen();
            int getItemId();
    };
}

#endif