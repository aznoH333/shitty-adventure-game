#ifndef STATE
#define STATE


#include "game/overworld/overworld.h"


enum GameState{
    STATE_MAIN_MENU = 0,
    STATE_OVERWORLD
};




class State{



    public:
        static State* get();
        void switchState(GameState state);
        void stateUpdate();
        void quitGame();
        bool shouldQuit();        
        void dispose();

    private:
        static State* instance;

        GameState currentState = STATE_MAIN_MENU;
        bool quit = false;
};


#endif