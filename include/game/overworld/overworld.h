#ifndef OVERWORLD 
#define OVERWORLD


#include "engine/all.h"
#include "overworldTerrain.h"






class OverworldPlayer{
    private:
        Vector2 subPosition;
        TerrainGeneration::OverworldPosition position;

    public:
        OverworldPlayer(TerrainGeneration::OverworldPosition position);
        void update();
};

class Overworld{
    private:
        static Overworld* instance;

        TerrainGeneration::OverworldTerrain* terrain;
        OverworldPlayer* player;

        Overworld();

    public:
        static Overworld* get();


        void init();
        void dispose();


        void update();

};



#endif