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

    enum TileSetType{
        TILESET_NONE = 0,
        TILESET_BASIC = 1,
        TILESET_ADVANCED = 2
    };
    
    
    struct OverworldTile{
        std::string sprite;
        bool blocksMovement;
        TileAnimation animation;
        int tileVariations;
        TileSetType type;
    };

    

    const OverworldTile tileLookupTable[] = {
            {"grass_tileset_", false, {false}, 2, TILESET_ADVANCED}, // grass
            {"sand_tileset_", false, {false}, 1, TILESET_ADVANCED}, // sand
            {"rock_tileset_", true, {false}, 2, TILESET_BASIC},  // mountain
            {"overworld_water", true, {true, 0, 2, 30}},   // water
            {"overworld_stone", false},   // stone
            {"overworld_empty", false},   // shore


    };
}



#endif