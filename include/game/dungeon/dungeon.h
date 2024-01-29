#ifndef DUNGEON 
#define DUNGEON 


#include "game/overworld/terrainStructs.h"
#include <vector>
#include <list>
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


    struct DungeonPlatformSpawner{
        Vector2 position;
        bool isWaterFall;
        int respawnTimer;
    };

    struct DungeonPlatform{
        Vector2 position;
        bool isFalling;
        float speed;
        bool isWaterFall;
        DungeonPlatformSpawner* spawner;
    };

    struct DungeonEnemy{
        Vector2 position;
        // temporary placeholder
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


            // platforms
            std::list<DungeonPlatform> platforms;
            std::vector<DungeonPlatformSpawner> platfromSpawners;

            const float WATERFALL_PLATFORM_SPEED = 0.6f;
            const float PLATFORM_ACCELERATION = 0.2;
            const float MAX_PLATFORM_SPEED = 2.0f;
            const int WATERFALL_PLATFORM_RESPAWN_TIME = 100;
            const int PLATFORM_RESPAWN_TIME = 200;
            const int PLATFORM_DESPAWN_TRESHOLD = 196;
            const float WATERFALL_PLATFORM_SPAWN_HEIGHT = 0.0f;


            // enemies
            std::list<DungeonEnemy> enemies;



            // generation values
            const int MIN_MAIN_SECTION_LENGTH = 200;
            const int MAX_MAIN_SECTION_LENGTH = 400;
            const int MIN_SECTION_LENGTH = 100;
            const int MAX_SECTION_LENGTH = 200;
            const int DUNGEON_PADDING = 10;


            // generation
            void unloadDungeon();
            void loadDungeon(int dungeonId, TerrainGeneration::OverworldPosition position);
            Level generateDungeon(TerrainGeneration::OverworldPosition position);
            DungeonSection generateSection(SectionPurpose);
            
            // entities
            void spawnEntity(Vector2 position, EntitySpawnType spawnType);
            EntitySpawnType getNextSpawnType(SpawnType type);

            // platforms
            void addPlatform(Vector2 position, bool isWaterFall);
            void updatePlatforms();
            void removeAllPlatforms();

            // enemies
            void addEnemy(Vector2 position);
            void updateEnemies();
            void removeAllEnemies();

            


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