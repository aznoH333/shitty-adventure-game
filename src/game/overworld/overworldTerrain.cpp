#include "game/overworld.h"



void OverworldTerrain::update(){
    for (int x = 0; x < 20; x++){
        for (int y = 0; y < 20; y++){
            Drawing::get()->drawTexture("grass", {x * 32.0f, y * 32.0f}, false, 1, 0, WHITE, DrawingLayers::LAYER_WORLD);
        }
    }
}
