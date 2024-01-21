#include "game/dungeon/dungeon.h"
#include "game/state.h"

// --== Boiler plate ==--
Dungeon* Dungeon::instance = 0;



Dungeon* Dungeon::get(){
    if (!instance){
        instance = new Dungeon();
    }
    return instance;
}


void Dungeon::dispose(){
    delete instance;
}



// --== update ==--

void Dungeon::update(){
    // todo something
}


// --== loading ==--
void Dungeon::enterDungeon(TerrainGeneration::OverworldPosition dungeonPosition){
    // Todo something
    State::get()->switchState(GameState::STATE_DUNGEON);
}