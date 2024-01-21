#ifndef DUNGEON 
#define DUNGEON 


#include "game/overworld/terrainStructs.h"




class Dungeon{

    private:
        static Dungeon* instance;



    public:
        static Dungeon* get();
        void update();
        void dispose();
        void enterDungeon(TerrainGeneration::OverworldPosition dungeonPosition);


};




#endif