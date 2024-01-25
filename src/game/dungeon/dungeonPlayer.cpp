#include "game/dungeon/dungeon.h"

using namespace DungeonCode;


DungeonPlayer::DungeonPlayer(Vector2 position){
    this->position = position;
}

void DungeonPlayer::update(){
    // temporary garbage update
    if (IsKeyDown(KEY_W)){
        position.y -= 1;
    }

    if (IsKeyDown(KEY_S)){
        position.y += 1;
    }

    if (IsKeyDown(KEY_A)){
        position.x -= 1;
    }

    if (IsKeyDown(KEY_D)){
        position.x += 1;
    }

    Drawing::get()->getCamera().target = position;
    Drawing::get()->drawTexture("dungeon_test_2", position, false, 1, 0, WHITE, DrawingLayers::LAYER_PLAYER);
}