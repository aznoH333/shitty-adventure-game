#ifndef DUNGEON 
#define DUNGEON 


#include "game/overworld/terrainStructs.h"
#include <vector>
#include <list>
#include "engine/utils.h"
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

    struct DungeonDoor{
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
        std::vector<DungeonPlatformSpawner> platfromSpawners;
        std::list<DungeonEnemy> enemies;
        std::list<DungeonDoor> doors;
    };

    struct Level {
        std::vector<DungeonSection> sections;
        int currentSection;
        int seed;
    };


    class DungeonPlayer{

        private:
            Vector2 position;
            Vector2 velocity = {0,0};
            const Vector2 SIZE = {16.0f, 16.0f};

            const float GRAVITY = 0.2f;
            const float JUMP_FORCE = 3.0f;
            const float WALK_ACCELERATION = 0.1f;
            const float WALK_SPEED = 2.0f;
            const int JUMP_HEIGHT_BUFFER_LENGTH = 10;

            
            // input vars
            bool buttonLeft = false;
            bool buttonRight = false;
            bool buttonJump = false;
            bool buttonJumpPressed = false;


            // movement vars
            const float ADDITIONAL_VELOCITY_DECAY_SPEED = 0.3f;
            const int JUMP_BUFFER_LENGTH = 10;
            bool isAirborne = false;
            float walkVal = 0.0f;
            float additionalHorizontalVelocity = 0.0f;
            int jumpHeightBuffer = 0;
            int jumpBuffer = 0;

            // wall climbing
            const float WALL_CLIMB_JUMP_FORCE = 3.2f;
            const float WALL_CLIMB_PUSH_FORCE = 2.5f;
            const float WALL_CLIMB_DECAY = 0.15f;
            bool canWallClimb = false;
            int wallClimbCounter = 0;

            // platform stuff
            DungeonPlatform* platformPtr = nullptr;

            



            void readPlayerInput();
            void tryMove();
            void tryWallClimbJump();
            void tryJump();
            void updateMovementValues();

        public:
            DungeonPlayer(Vector2 position);
            void update();
    };

    const int DUNGEON_TILE_SIZE = 16;

    class Dungeon{

        private:
            static Dungeon* instance;
            TerrainGeneration::OverworldObject* overworldDungeon;
            Level currentLoadedLevel;
            DungeonPlayer* player = nullptr;



            const float CAMERA_Y = 86.0f;
            const float MIN_CAMERA_X = 256.0f; 
            const Vector2 PLATFORM_SIZE = {48.0f, 10.0f};       

            
            int nextDungeonId = 1;
            int lastGeneratedDungeonId = 0;
            int currentSeed = 0;


            // platforms
            std::list<DungeonPlatform> platforms;

            const float WATERFALL_PLATFORM_SPEED = 0.6f;
            const float PLATFORM_ACCELERATION = 0.1f;
            const float MAX_PLATFORM_SPEED = 4.5f;
            const int WATERFALL_PLATFORM_RESPAWN_TIME = 100;
            const int PLATFORM_RESPAWN_TIME = 200;
            const int PLATFORM_DESPAWN_TRESHOLD = 212;
            const float WATERFALL_PLATFORM_SPAWN_HEIGHT = 0.0f;
            


            // enemies



            // generation values
            const int MIN_MAIN_SECTION_LENGTH = 200;
            const int MAX_MAIN_SECTION_LENGTH = 400;
            const int MIN_SECTION_LENGTH = 100;
            const int MAX_SECTION_LENGTH = 200;
            const int DUNGEON_PADDING = 10;
            const int MIN_TILE_GAP = 2;
            const int TEMPORARY_MAX_JUMP_HEIGHT = 5;
            const int TEMPORARY_MAX_JUMP_HEIGHT_AFTER_GAP = 2;

            std::map<int, std::vector<int>> dynamicConnectorLookupTable;

            // generation
            void unloadDungeon();
            void loadDungeon(int dungeonId, TerrainGeneration::OverworldPosition position);
            Level generateDungeon(TerrainGeneration::OverworldPosition position);
            DungeonSection generateSection(SectionPurpose);
            std::vector<int>& getPossibleConnectors(int currentId);
            
            // entities
            void spawnEntity(Vector2 position, EntitySpawnType spawnType, DungeonSection& section);
            EntitySpawnType getNextSpawnType(SpawnType type);

            // platforms
            void addPlatform(Vector2 position, bool isWaterFall, std::vector<DungeonPlatformSpawner>& platfromSpawners);
            void updatePlatforms();
            void removeAllPlatforms();

            // enemies
            void addEnemy(Vector2 position, DungeonSection& section);
            void updateEnemies();

            // door stuff
            void addDoor(Vector2 position, DungeonSection& section);
            void updateDoors();

            
            // collisions
            bool collidesWithLevel(Vector2& position, Vector2& size);
            bool collidesWithPlatform(Vector2& position, Vector2& size);
            bool collidesWithPlatformAdvanced(Vector2& position, Vector2& size, Vector2& actualPosition, DungeonPlatform*& platformPointerRef);



        public:
            Dungeon();
            static Dungeon* get();
            void update();
            void draw();
            void dispose();
            void enterDungeon(TerrainGeneration::OverworldObject* dungeon);
            void setCameraPosition(Vector2 playerPosition);
            bool collidesWithDungeon(Vector2 position, Vector2 size);
            bool collidesWithDungeon(Vector2 position, Vector2 size, bool checkPlatforms);

            bool advancedDungeonCollisions(Vector2 position, Vector2 size, Vector2& actualPosition, DungeonPlatform*& platformPointerRef);

    };
}




#endif