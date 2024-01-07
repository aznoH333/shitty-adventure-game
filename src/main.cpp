#include <raylib-cpp.hpp>
#include "engine/drawing.h"




const int windowWidth = 1280;
const int windowHeight = 720;

int main() {
    
    // Initialization
    raylib::Window w(windowWidth, windowHeight, "shitty adventure game");

    Drawing::init("./assets/", 10, windowWidth, windowHeight);
    

    
    SetTargetFPS(60);
    while (!w.ShouldClose())
    {
        // main loop        
        Drawing::get()->render();
        Drawing::get()->drawTexture("player_1", {100, 100}, false, 1, 0, WHITE, 0);
        
    }
    
    Drawing::get()->dispose();
    


    return 0;
}