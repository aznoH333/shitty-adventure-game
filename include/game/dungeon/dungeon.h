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
            
            
            int nextDungeonId = 1;
            int lastGeneratedDungeonId = 0;
            int currentSeed = 0;


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


        public:
            Dungeon();
            static Dungeon* get();
            void update();
            void draw();
            void dispose();
            void enterDungeon(TerrainGeneration::OverworldObject* dungeon);


    };
}




#endif