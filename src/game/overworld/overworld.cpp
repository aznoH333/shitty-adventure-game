#include "game/overworld.h"

// --== boiler plate ==--


Overworld* Overworld::instance = 0;



Overworld* Overworld::get(){
    if (!instance){
        instance = new Overworld();
    }

    return instance;
}

void Overworld::dispose(){
    delete instance;
}



// --== logic ==--
void Overworld::update(){
    terrain.update();
    player.update();
}