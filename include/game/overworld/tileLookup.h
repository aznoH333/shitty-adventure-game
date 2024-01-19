#ifndef TILE_LOOKUP
#define TILE_LOOKUP

#include <string>

namespace TerrainGeneration {
    struct OverworldTile{
        std::string sprite;
        bool blocksMovement;
    };

    const OverworldTile tileLookupTable[] = {
            {"test_art_4_5", false}, // grass
            {"test_art_4_7", false}, // sand
            {"test_art_4_4", true},  // mountain
            {"test_art_4_9", true},   // water
            {"test_art_4_8", false},   // stone
            {"test_art_4_6", false},   // shore


    };
}



#endif