#ifndef OVERWORLD 
#define OVERWORLD


#include "engine/all.h"



class OverworldTerrain{
    private:

    public:
        void update();
};

class OverworldPlayer{
    public:
        void update();
};

class Overworld{
    private:
        static Overworld* instance;

        OverworldTerrain terrain;
        OverworldPlayer player;

    public:
        static Overworld* get();
        void dispose();


        void update();

};



#endif