#ifndef TERRAIN_STRUCTS
#define TERRAIN_STRUCTS

#include <string>
#include <vector>
#include "game/dungeon/dungeonTheme.h"


namespace TerrainGeneration {
    const int OVERWORLD_CHUNK_SIZE = 16;


    struct OverworldPosition{
        bool operator==(const OverworldPosition &other) const {
            return x == other.x && y == other.y;
        }

        bool operator<(const OverworldPosition &other) const {
            return x < other.x || (x == other.x && y < other.y);
        }

        int x;
        int y;
    };

    struct ChunkCoordinates{

        bool operator==(const ChunkCoordinates &other) const {
            return x == other.x && y == other.y;
        }

        bool operator<(const ChunkCoordinates &other) const {
            return x < other.x || (x == other.x && y < other.y);
        }

        int x;
        int y;
    };



    
    enum ObjectType{
        OBJECT_DUNGEON,
        OBJECT_TREE,
    };

    // tree, castle, dungeon, village ect
    struct OverworldObject{
        std::string sprite;
        OverworldPosition position;
        ObjectType type;
        int dungeonId = 0;
        bool hasAssignedId = false;
        DungeonCode::DungeonTheme* theme;

    };

    enum PatternGenrationType{
        PATTERN_WATER,
        PATTERN_DUNGEON,
        PATTERN_TREE,
    };


    struct PatternGenerationObject{
        OverworldPosition position;
        PatternGenrationType patern;
    };

    struct InWorldTile{
        int tileId;
        int spriteVariant;
    };

    struct OverworldChunk{
        ChunkCoordinates coordinates;
        InWorldTile tiles[OVERWORLD_CHUNK_SIZE][OVERWORLD_CHUNK_SIZE];
        std::vector<OverworldObject> worldObjects;
        std::vector<PatternGenerationObject> patterns;
        int terrainBiome;
    };

    struct TileSurroundInfo{
        bool up;
        bool down;
        bool left;
        bool right;
    };
}

#endif