#ifndef OVERWORLD 
#define OVERWORLD


#include "engine/all.h"
#include "overworldTerrain.h"
#include "terrainStructs.h"
#include "game/ui/ui.h"


enum Direction{
    DIRECTION_UP,
    DIRECTION_DOWN,
    DIRECTION_LEFT,
    DIRECTION_RIGHT,
};


class OverworldPlayer{
    private:
        Vector2 subPosition;
        TerrainGeneration::OverworldPosition position;
        const float MOVE_SPEED = 0.05f;
        const float OBJECT_REACH = 2.0f;
        Direction direction = DIRECTION_DOWN;
        bool isMoving = false;

        

        TerrainGeneration::OverworldObject* nearbyDungeon;
        bool isDungeonNearby = false;


        void movedToAnotherTile();
        void tryMove(Vector2 moveBy);\
        void draw();
        int getWalkSprite(int input);

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


        void init(int seed);
        void dispose();
        void setPlayerChunk(TerrainGeneration::ChunkCoordinates coordinates);
        bool collidesWithTerrain(Vector2 position, Vector2 size);
        std::vector<TerrainGeneration::OverworldObject*> getNearbyObjects(TerrainGeneration::OverworldPosition position, float radius);



        void update();
        TerrainGeneration::OverworldTerrain* getTerrain();

};


// utility functions
Vector2 overworldPosToVec(TerrainGeneration::OverworldPosition position);


#endif