#include "game/overworld/overworldTerrain.h"
#include "engine/utils.h"

using namespace Utils;
namespace TerrainGeneration {

    OverworldObject generateDungeon(const TerrainBiome& biome, OverworldPosition position){
        
        int themeMax = biome.dungeonTypes.size() - 1;
        int seed = hashVector(position.x, position.y);
        DungeonCode::DungeonTheme* theme = &DungeonCode::themeLookup.at(biome.dungeonTypes.at(getPseudoRandomInt(0, themeMax, seed)));

        return {
            .sprite = theme->getOverworldSprite(),
            .position = position,
            .type = OBJECT_DUNGEON,
            .theme = theme,
        };
    }


   

    OverworldObject generateTree(const TerrainBiome& biome, OverworldPosition position, int treeType){
        float rng = Utils::getPseudoRandomFloat(Utils::hashVector(position.x, position.y) + 1984);
        
        OverworldObject output = {
            .sprite = "overworld_tree_0",
            .position = position,
            .type = OBJECT_TREE,
            .theme = nullptr,
        };
        
        switch (treeType){
            default:
                output.sprite = "overworld_tree_0";
                break;
            case 1:
                if (rng < 0.5f){
                    output.sprite = "overworld_tree_1";
                }else {
                    output.sprite = "overworld_tree_2";
                }
                break;
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

