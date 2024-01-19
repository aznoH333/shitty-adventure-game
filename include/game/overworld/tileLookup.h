#ifndef TILE_LOOKUP
#define TILE_LOOKUP

#include <string>

namespace TerrainGeneration {
    
    struct TileAnimation{
        bool animates = false;
        int startFrame = 0;
        int endFrame = 0;
        int ticksPerFrame = 60;

    };
    
    
    struct OverworldTile{
        std::string sprite;
        bool blocksMovement;
        TileAnimation animation;
    };

    

    const OverworldTile tileLookupTable[] = {
            {"overworld_grass", false}, // grass
            {"overworld_sand", false}, // sand
            {"overworld_rock", true},  // mountain
            {"overworld_water", true, {true, 0, 2, 30}},   // water
            {"overworld_stone", false},   // stone
            {"overworld_empty", false},   // shore


    };
}



#endif