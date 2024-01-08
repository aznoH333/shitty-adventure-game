#include "game/overworld.h"


void OverworldPlayer::update(){
    Drawing::get()->drawTexture("player", {0.0f, 0.0f}, false, 1, 0, WHITE, DrawingLayers::LAYER_PLAYER);
}