#ifndef BIOME_LOOKUP
#define BIOME_LOOKUP

#include "game/dungeon/dungeonTheme.h"
#include <vector>

namespace TerrainGeneration {


    struct TerrainBiome{
        float mountainValue;
        float stoneValue;
        float shoreValue;
        float waterGenerationValue;
        float treeGenerationValue;
        float structureFrequency;
        float preferedTreeValue;
        float treeToleranceFalloff;
        float treeDensity;
        int terrainRoughnessModifier;



        int mountainTile;
        int stoneTile;
        int shoreTile;
        int defaultTile;
        int sandTile;
        int waterTile;
        int structureType;
        int treeType;
        std::vector<DungeonCode::DungeonThemeType> dungeonTypes;
        
    };



    const TerrainBiome biomeLookup[] = {
        // default biome
        {
            .mountainValue = 0.7,
            .stoneValue = 0.6,
            .shoreValue = 0.35,
            .waterGenerationValue = 0.3f,
            .treeGenerationValue = 0.5f,
            .structureFrequency = 20,
            .preferedTreeValue = 0.7f,
            .treeToleranceFalloff = 0.9f,
            .treeDensity = 0.25f,
            .terrainRoughnessModifier = 5,


            .mountainTile = 2,
            .stoneTile = 4,
            .shoreTile = 0,
            .defaultTile = 0,
            .sandTile = 1,
            .waterTile = 3,
            .structureType = 0,
            .treeType = 0,
            .dungeonTypes = {DungeonCode::THEME_DEFAULT,DungeonCode::THEME_ALT,DungeonCode::THEME_GREEN,DungeonCode::THEME_MUD},
        },
        // desert biome
        {
            .mountainValue = 0.60f,
            .stoneValue = 0.55f,
            .shoreValue = 0.20f,
            .waterGenerationValue = 0.2f,
            .treeGenerationValue = 0.7f,
            .structureFrequency = 20,
            .preferedTreeValue = 0.5f,
            .treeToleranceFalloff = 1.1f,
            .treeDensity = 0.15f,
            .terrainRoughnessModifier = 10,


            .mountainTile = 2,
            .stoneTile = 4,
            .shoreTile = 5,
            .defaultTile = 1,
            .sandTile = 1,
            .waterTile = 3,
            .structureType = 0,
            .treeType = 1,
            .dungeonTypes = {DungeonCode::THEME_DEFAULT, DungeonCode::THEME_MUD, DungeonCode::THEME_SAND},

        },
    };

}


#endif