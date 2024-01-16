

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
            .shoreValue = 0.4,
            .waterGenerationValue = 0.3,
            .treeGenerationValue = 0.4,
            .structureFrequency = 20,


            .mountainTile = 2,
            .stoneTile = 1,
            .shoreTile = 0,
            .defaultTile = 0,
            .sandTile = 1,
            .waterTile = 3,
            .structureType = 0,
        }
    };

}

#endif