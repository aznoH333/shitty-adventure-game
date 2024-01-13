#include <raylib-cpp.hpp>
#include "engine/all.h"
#include "game/all.h"


const int windowWidth = 1280;
const int windowHeight = 720;


int main() {
    
    // Initialization
    raylib::Window w(windowWidth, windowHeight, "shitty adventure game");
    Drawing::init("./assets/", 10, windowWidth, windowHeight);


    // init game
    State::get()->switchState(STATE_OVERWORLD);



    SetTargetFPS(60);
    while (!w.ShouldClose() && !State::get()->shouldQuit())
    {
        // main loop        
        Drawing::get()->render();
        State::get()->stateUpdate();
        
        //Drawing::get()->drawTexture("player_1", {100, 100}, false, 1, 0, WHITE, 0);
        
    }
    

    Drawing::get()->dispose();
    State::get()->dispose();
    Overworld::get()->dispose();
    


    return 0;
}