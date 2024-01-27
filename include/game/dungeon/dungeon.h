#ifndef DUNGEON 
#define DUNGEON 


#include "game/overworld/terrainStructs.h"
#include <vector>
#include "engine/drawing.h"
#include "dungeonTileLookup.h"
#include "raylib.h"

namespace DungeonCode {


    enum SectionPurpose{
        MAIN_SECTION,
        KEY_SECTION,
        LOOT_SECTION,
        NOTHING_SECTION,
    };

    enum EntitySpawnType{
        NONE_SPAWN = 0,
        PLAYER_SPAWN,
        DOOR_SPAWN,
        ITEM_SPAWN,
        PLATFORM_SPAWN,
        WATERFALL_SPAWN,
        ENEMY_SPAWN,
    };

    struct LevelSlice{
        int geometryPattern;
        int tilesetModifier;
    };

    struct DungeonSection{
        std::vector<LevelSlice> levelData;
        int sectionLength;
        SectionPurpose purpose;
    };

    struct Level {
        std::vector<DungeonSection> sections;
        int currentSection;
        int seed;
    };


    class DungeonPlayer{

        private:
            Vector2 position;

        public:
            DungeonPlayer(Vector2 position);
            void update();
    };


    class Dungeon{

        private:
            static Dungeon* instance;
            TerrainGeneration::OverworldObject* overworldDungeon;
            Level currentLoadedLevel;
            DungeonPlayer* player = nullptr;



            const int DUNGEON_TILE_SIZE = 16;
            const float CAMERA_Y = 56.0f;
            const float MIN_CAMERA_X = 152.0f;            
            
            int nextDungeonId = 1;
            int lastGeneratedDungeonId = 0;
            int currentSeed = 0;

            // generation vals
            int currentSegmentId = 0;
            int remainingSegmantLength = 0;
            int nextSpawnIndex = 0;
            float nextSpawnY = 0.0f;

            EntitySpawnType nextSpawnType = NONE_SPAWN; 
            bool playerSpawned = false;


            // generation values
            const int MIN_MAIN_SECTION_LENGTH = 200;
            const int MAX_MAIN_SECTION_LENGTH = 400;
            const int MIN_SECTION_LENGTH = 100;
            const int MAX_SECTION_LENGTH = 200;
            const int DUNGEON_PADDING = 10;

            void unloadDungeon();
            void loadDungeon(int dungeonId, TerrainGeneration::OverworldPosition position);
            Level generateDungeon(TerrainGeneration::OverworldPosition position);
            DungeonSection generateSection(SectionPurpose);
            LevelSlice getNextSegment(int index, const DungeonSection* level);
            void spawnEntity(Vector2 position, EntitySpawnType spawnType);
            EntitySpawnType getNextSpawnType(SpawnType type);

            


        public:
            Dungeon();
            static Dungeon* get();
            void update();
            void draw();
            void dispose();
            void enterDungeon(TerrainGeneration::OverworldObject* dungeon);
            void setCameraPosition(Vector2 playerPosition);

    };
}




#endif