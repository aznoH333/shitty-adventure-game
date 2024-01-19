#include "game/overworld/overworldTerrain.h"

namespace TerrainGeneration {

    OverworldObject generateDungeon(const TerrainBiome& biome, OverworldPosition position){
        
        return {
            .sprite = "dungeon_1",
            .position = position,
            .type = OBJECT_DUNGEON,
        };
    }


   

    OverworldObject generateTree(const TerrainBiome& biome, OverworldPosition position, int treeType){
        float rng = Utils::getPseudoRandomFloat(Utils::hashVector(position.x, position.y) + 1984);
        
        OverworldObject output = {
            .sprite = "overworld_tree_0",
            .position = position,
            .type = OBJECT_TREE,
        };
        
        switch (treeType){
            default:
                output.sprite = "overworld_tree_0";
            case 1:
                if (rng < 0.5f){
                    output.sprite = "overworld_tree_1";
                }else {
                    output.sprite = "overworld_tree_2";
                }
        }

        return output;
        
        
    }


    




    OverworldObject OverworldTerrain::generateOverworldObject(const TerrainBiome& biome, ObjectType type, OverworldPosition position){
        switch (type) {
            case OBJECT_DUNGEON:
                return generateDungeon(biome, position);
            case OBJECT_TREE:
                return generateTree(biome, position, biome.treeType);
        }
        return generateTree(biome, position, biome.treeType);
    }

}

