#include "game/overworld/overworldTerrain.h"

namespace TerrainGeneration {

    OverworldObject generateDungeon(const TerrainBiome& biome, OverworldPosition position){
        
        return {
            .sprite = "test_art_4_3",
            .position = position,
            .type = OBJECT_DUNGEON,
        };
    }

    OverworldObject generateTree(const TerrainBiome& biome, OverworldPosition position){
        return {
            .sprite = "test_art_4_2",
            .position = position,
            .type = OBJECT_TREE,
        };
    }


    




    OverworldObject OverworldTerrain::generateOverworldObject(const TerrainBiome& biome, ObjectType type, OverworldPosition position){
        switch (type) {
            case OBJECT_DUNGEON:
                return generateDungeon(biome, position);
            case OBJECT_TREE:
                return generateTree(biome, position);
        }
        return generateTree(biome, position);
    }

}

