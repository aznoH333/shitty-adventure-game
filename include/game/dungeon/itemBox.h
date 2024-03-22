#ifndef ITEM_BOX
#define ITEM_BOX

#include "raylib.h" 

namespace DungeonCode {
    class ItemBox{
        private:
            Vector2 position;
            int item;
            bool isOpen = false;
            float itemOffset = 0.0f;
            const float OFFSET_IN_PIXELS = 10.0f;
            const float OFFSET_SPEED = 0.02f;
            const float OFFSET_X = 8.0f; 

        public:
            ItemBox(Vector2 position, int item);
            void update();
            void switchItem(int* otherItem);
            Vector2& getPosition();
            void openBox();
            bool isBoxOpen();
    };
}

#endif