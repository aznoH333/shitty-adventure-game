#include <raylib-cpp.hpp>
#include "engine/all.h"
#include "game/all.h"


const int windowWidth = 1280;
const int windowHeight = 720;

int Utils::gameTickCounter = 0; // lmao

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
        Utils::gameTickCounter++; // pavle naser si

        Drawing::get()->render();
        State::get()->stateUpdate();
        



        
    }
    

    Drawing::get()->dispose();
    State::get()->dispose();
    Overworld::get()->dispose();
    Dungeon::get()->dispose();
    


    return 0;
}