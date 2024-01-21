#include "game/state.h"

// --== boiler plate ==--
State* State::instance = 0;


State* State::get(){
    if (instance == nullptr){
        instance = new State();
    }
    return instance;
}

void State::dispose(){
    delete instance;
}




// --== state management == --
void State::switchState(GameState state){
    currentState = state;
}

void State::stateUpdate(){
    switch (currentState) {
        case GameState::STATE_OVERWORLD:
            Overworld::get()->update();
            break;
        case GameState::STATE_MAIN_MENU:
            // TODO
            break;
        case GameState::STATE_DUNGEON:
            Dungeon::get()->update();
        

    }
}



// --== quit game ==--
void State::quitGame(){
    quit = true;
}
bool State::shouldQuit(){
    return quit;
}     