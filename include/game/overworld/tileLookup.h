#ifndef TILE_LOOKUP
#define TILE_LOOKUP

#include <string>

namespace TerrainGeneration {
    struct OverworldTile{
        std::string sprite;
        bool blocksMovement;
    };

    const OverworldTile tileLookupTable[] = {
            {"placeholders_1", false}, // grass
            {"placeholders_2", false}, // sand
            {"placeholders_3", true},  // mountain
            {"placeholders_4", true}   // water
    };
}



#endif