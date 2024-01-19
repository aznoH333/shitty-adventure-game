

#ifndef BIOME_LOOKUP
#define BIOME_LOOKUP

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
    };



    const TerrainBiome biomeLookup[] = {
        // default biome
        {
            .mountainValue = 0.7,
            .stoneValue = 0.6,
            .shoreValue = 0.35,
            .waterGenerationValue = 0.3,
            .treeGenerationValue = 0.7,
            .structureFrequency = 20,
            .preferedTreeValue = 0.5f,
            .treeToleranceFalloff = 1.1f,
            .treeDensity = 0.25f,
            .terrainRoughnessModifier = 5,


            .mountainTile = 2,
            .stoneTile = 4,
            .shoreTile = 5,
            .defaultTile = 0,
            .sandTile = 1,
            .waterTile = 3,
            .structureType = 0,
        }
    };

}

#endif