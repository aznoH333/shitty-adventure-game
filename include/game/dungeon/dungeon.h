#ifndef DUNGEON 
#define DUNGEON 


#include "game/overworld/terrainStructs.h"
#include <vector>
#include <list>
#include <map>
#include "engine/utils.h"
#include "engine/drawing.h"
#include "dungeonTileLookup.h"
#include "raylib.h"
#include "game/ui/ui.h"
#include "engine/sound.h"
#include "dungeonEnemy.h"
#include "game/stats/playerStats.h"
#include "itemBox.h"

namespace DungeonCode {


    enum SectionPurpose{
        MAIN_SECTION,
        FILLER_SECTION,
        REWARD_SECTION,
        FIGHT_SECTION,
    };

    struct SectionInfo{
        SectionPurpose purpose;
        bool isMain;
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

    

    struct DungeonDoor{
        Vector2 position;
        Vector2 exitLocation;
        int target;
        bool useDefaultEntry;
        // temporary placeholder
    };

    

    enum EntitySpawnType{
        NONE_SPAWN = 0,
        PLAYER_SPAWN,
        DOOR_SPAWN,
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
        SectionInfo sectionInfo;
        std::vector<DungeonPlatformSpawner> platfromSpawners;
        std::list<DungeonEnemy> enemies;
        std::list<DungeonDoor> doors;
        std::list<ItemBox> boxes;
        Vector2 defaultEntry;
    };

    struct Level {
        std::map<int, DungeonSection> sections;
        int currentSection;
        int seed;
        int lastId;
    };

    
    


    class DungeonPlayer{

        private:
            enum GunOperationState{
                IDLE = 0,
                FIRING,
                RELOADING,
            };
            struct GunState {

                float direction = 0;
                int shotLoadTimer = 0;
                int recoilTimer = 0;
                bool shouldSpawnShell = false;
                Vector2 position;
                GunOperationState currentState = IDLE;
                int reloadTimer = 0;
                int reloadPrepTimer = 0;
                bool playRackSoundEffect = false;


            };
            
            Vector2 position;
            Vector2 velocity = {0,0};

            const float GRAVITY = 0.3f;
            const float JUMP_FORCE = 3.0f;
            const float WALK_ACCELERATION = 0.1f;
            const float WALK_SPEED = 2.0f;
            const int JUMP_HEIGHT_BUFFER_LENGTH = 10;
            bool flipSprite = false;

            
            // input vars
            bool buttonLeft = false;
            bool buttonRight = false;
            bool buttonJump = false;
            bool buttonJumpPressed = false;
            bool buttonInteractPressed = false;
            bool buttonFirePressed = false;
            bool buttonReloadPressed = false;

            // movement vars
            const float ADDITIONAL_VELOCITY_DECAY_SPEED = 0.3f;
            const int WALK_ANIMATION_START_INDEX = 1;
            const int WALK_ANIMATION_END_INDEX = 3;
            const int WALK_ANIMATION_FRAME_DURATION = 7;

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

            // gun
            GunState gunState = {};
            const float GUN_OFFSET_Y = 10.0f;
            const float GUN_OFFSET_X_1 = 4.0f;
            const float GUN_OFFSET_X_2 = 12.0f;
            const int RECOIL_TIME = 15;
            const float RECOIL_OFFSET = 35.5f;
            const Vector2 SHELL_EJECT_VELOCITY = {4.5f, -4.0f};
            const float SHELL_ROTATION_SPEED = 4.0f;
            
            // reloading
            const int RELOAD_PREP_MAX = 10;
            const float RELOAD_PREP_OFFSET = 30.0f;
            const float RELOAD_INSERT_OFFSET = 10.0f;

            // box interaction
            void updateBoxInteraction();

            // post hit invincibility
            int postHitInvincibility = 0;
            const int MAX_POST_HIT_INVINCIBILITY = 90;


            const float BULLET_SPAWN_OFFSET = 16.0f;


            // platform stuff
            DungeonPlatform* platformPtr = nullptr;


            // door stuff
            DungeonDoor* nearbyDoor = nullptr;
            



            void readPlayerInput();
            void tryMove();
            void tryWallClimbJump();
            void tryJump();
            void updateMovementValues();
            void updateGravity();
            void updateWallClimbing();
            void updateDoorInteraction();
            void drawSprite();
            void updateGun();
            void drawGun();

        public:
            static constexpr Vector2 SIZE = {16.0f, 25.0f};

            DungeonPlayer(Vector2 position);
            Vector2& getPosition();
            void setNearbyDoor(DungeonDoor* door);
            void update();
            void takeDamage(int damage);
    };

    const int DUNGEON_TILE_SIZE = 16;

    struct Giblet{
        std::string sprite;
        Vector2 position;
        Vector2 velocity;
        float rotationSpeed;
        float rotation;
        int gibletType;
    };

    struct Projectile{
        std::string sprite;
        Vector2 position;
        Vector2 velocity;
        float rotation;
        int extraUpdates = 1;
        bool alliedWithPlayer;
        float damage;
    };





    class Dungeon{

        private:
            static Dungeon* instance;
            TerrainGeneration::OverworldObject* overworldDungeon;
            Level currentLoadedLevel;
            DungeonPlayer* player = nullptr;



            const float CAMERA_Y = 109.0f;
            const float MIN_CAMERA_X = 256.0f; 
            const Vector2 PLATFORM_SIZE = {48.0f, 10.0f};       

            
            int nextDungeonId = 1;
            int lastGeneratedDungeonId = 0;
            int currentSeed = 0;


            // platforms
            std::list<DungeonPlatform> platforms;

            // giblets
            std::list<Giblet> giblets;
            const float GIBLET_GRAVITY = 0.3f;


            // projectiles
            std::list<Projectile> projectiles;
            const Vector2 PROJECTILE_SIZE = {8.0f, 8.0f};

            const float WATERFALL_PLATFORM_SPEED = 0.6f;
            const float PLATFORM_ACCELERATION = 0.1f;
            const float MAX_PLATFORM_SPEED = 4.5f;
            const int WATERFALL_PLATFORM_RESPAWN_TIME = 100;
            const int PLATFORM_RESPAWN_TIME = 200;
            const int PLATFORM_DESPAWN_TRESHOLD = 212;
            const float WATERFALL_PLATFORM_SPAWN_HEIGHT = 0.0f;
            const float NEARBY_DOOR_DISTANCE = 32.0f;
            
            // generation values
            const int MIN_MAIN_SECTION_LENGTH = 200;
            const int MAX_MAIN_SECTION_LENGTH = 400;
            const int MIN_SECTION_LENGTH = 100;
            const int MAX_SECTION_LENGTH = 200;
            const int DUNGEON_PADDING = 10;
            const int MIN_TILE_GAP = 4;
            const int TEMPORARY_MAX_JUMP_HEIGHT = 5;
            const int TEMPORARY_MAX_JUMP_HEIGHT_AFTER_GAP = 2;
            const int REWARD_ROOM_SIZE = 25;
            const int SMALL_REWARD_ROOM_SIZE = 10;
            const float SIDE_ROOM_CHANCE = 0.03f;
            const int FIGHT_ROOM_SIZE = 35;

            // Boxes
            const Vector2 BOX_SIZE = {32, 32};
            const float MAX_DISTANCE_TO_BOX = 32.0f;

            std::map<int, std::vector<int>> dynamicConnectorLookupTable;

            // generation
            void unloadDungeon();
            void loadDungeon(int dungeonId, TerrainGeneration::OverworldPosition position);
            Level generateDungeon(TerrainGeneration::OverworldPosition position);
            DungeonSection generateSection(SectionInfo info, Level& level, int returnValue, int sectionId, Vector2 returnLoacation);
            std::vector<int>& getPossibleConnectors(int currentId);
            SectionInfo getNextPurpose(SectionInfo info);

            // tile generating functions
            void addPadding(DungeonSection& section);
            void generateRegularLevel(DungeonSection& section, Level& level, int returnSection, int sectionId, Vector2 returnLoacation);
            void generateMainSection(DungeonSection& section, Level& level, int returnSection, int sectionId, Vector2 returnLoacation);
            void generateFightSection(DungeonSection& section, Level& level, int returnSection, int sectionId, Vector2 returnLoacation);
            void generateFillerSection(DungeonSection& section, Level& level, int returnSection, int sectionId, Vector2 returnLoacation);
            void generateRewardSection(DungeonSection& section, Level& level, int returnSection, int sectionId, Vector2 returnLoacation);
            void generateGeneric(DungeonSection& section, Level& level, int returnSection, int sectionId, Vector2 returnLoacation, bool isMain);

            void clearTemporaryObjects();


            // entities
            void spawnEntity(Vector2 position, EntitySpawnType spawnType, DungeonSection& section, Level& level, int doorTarget, Vector2 returnLocation, bool useDefaultEntry);
            EntitySpawnType getNextSpawnType(SpawnType type);

            // platforms
            void addPlatform(Vector2 position, bool isWaterFall, std::vector<DungeonPlatformSpawner>& platfromSpawners);
            void updatePlatforms(DungeonSection& section);
            void removeAllPlatforms();

            // enemies
            void addEnemy(Vector2 position, DungeonSection& section);
            void updateEnemies(DungeonSection& section);

            // door stuff
            void addDoor(Vector2 position, int targetSection, DungeonSection& section, Vector2 returnLocation, bool useDefaultEntry);
            void updateDoors(DungeonSection& section);

            
            // collisions
            bool collidesWithLevel(Vector2& position, Vector2& size);
            bool collidesWithPlatform(Vector2& position, Vector2& size);
            bool collidesWithPlatformAdvanced(Vector2& position, Vector2& size, Vector2& actualPosition, DungeonPlatform*& platformPointerRef);

            // boxes
            void updateBoxes();

            // giblets
            void updateGiblets();
            void clearAllGiblets();

            // projectiles
            void updateProjectiles();
            void clearAllProjectiles();

            

        public:
            Dungeon();
            static Dungeon* get();
            void update();
            void draw(DungeonSection& section);
            void dispose();
            void enterDungeon(TerrainGeneration::OverworldObject* dungeon);
            void setCameraPosition(Vector2 playerPosition);
            bool collidesWithDungeon(Vector2 position, Vector2 size);
            bool collidesWithDungeon(Vector2 position, Vector2 size, bool checkPlatforms);
            void enterDoor(DungeonDoor* door);
            void exitDungeon();
            void addGiblet(Giblet giblet);
            void addProjectile(Projectile projectile);
            bool advancedDungeonCollisions(Vector2 position, Vector2 size, Vector2& actualPosition, DungeonPlatform*& platformPointerRef);
            ItemBox* lookForNearbyBoxes(Vector2 position);
            DungeonPlayer* getPlayer();
    };


}




#endif