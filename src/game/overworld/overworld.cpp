#include "game/overworld/overworld.h"

// --== boiler plate ==--


Overworld* Overworld::instance = 0;



Overworld* Overworld::get(){
    if (!instance){
        instance = new Overworld();
    }

    return instance;
}

Overworld::Overworld(){
    player = new OverworldPlayer({0,0});
    terrain = new TerrainGeneration::OverworldTerrain();
}

void Overworld::dispose(){
    delete player;
    delete terrain;
    delete instance;
    
}



// --== logic ==--
void Overworld::update(){
    terrain->update();
    player->update();
}