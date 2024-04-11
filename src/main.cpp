#include <raylib-cpp.hpp>
#include "engine/all.h"
#include "game/all.h"
#include <iostream>
#include "engine/sound.h"

const int windowWidth = 1280;
const int windowHeight = 720;
const int SEED = 704641;

int Utils::gameTickCounter = 0; // lmao

int main() {
    
    // Initialization
    raylib::Window w(windowWidth, windowHeight, "shitty adventure game");
    Drawing::init("./assets/", 10, windowWidth, windowHeight);
    Audio::get()->loadAllSounds("./assets/sounds/");
    // init game
    State::get()->switchState(STATE_OVERWORLD);
    PlayerStats::initDefault();
    Overworld::get()->init(SEED);
    DungeonCode::EnemyManager::get()->init(SEED);


    SetTargetFPS(60);
    while (!w.ShouldClose() && !State::get()->shouldQuit())
    {
        // main loop
        Utils::gameTickCounter++; // pavle naser si
        UICode::updateUi();
        Drawing::get()->render();
        State::get()->stateUpdate();
        
        
    }
    

    Drawing::get()->dispose();
    State::get()->dispose();
    Overworld::get()->dispose();
    DungeonCode::Dungeon::get()->dispose();
    Audio::dispose();
    PlayerStats::dispose();
    


    return 0;
}