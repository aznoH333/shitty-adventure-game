#ifndef OVERWORLD 
#define OVERWORLD


#include "engine/all.h"
#include "overworldTerrain.h"
#include "terrainStructs.h"






class OverworldPlayer{
    private:
        Vector2 subPosition;
        TerrainGeneration::OverworldPosition position;
        const float moveSpeed = 0.05f;


        void tryMove(Vector2 moveBy);

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
        bool collidesWithTerrain(Vector2 position, Vector2 size);



        void update();

};



#endif