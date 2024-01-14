#ifndef OVERWORLD 
#define OVERWORLD


#include "engine/all.h"
#include "overworldTerrain.h"
#include "terrainStructs.h"






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
        TerrainGeneration::ChunkCoordinates playerChunk = {0,0};

        Overworld();

    public:
        static Overworld* get();


        void init();
        void dispose();
        void setPlayerChunk(TerrainGeneration::ChunkCoordinates coordinates);



        void update();

};



#endif