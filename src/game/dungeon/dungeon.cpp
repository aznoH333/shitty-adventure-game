#include "game/dungeon/dungeon.h"
#include "game/state.h"
#include <iostream>

namespace DungeonCode {


    // --== Boiler plate ==--
    Dungeon* Dungeon::instance = 0;



    Dungeon* Dungeon::get(){
        if (!instance){
            instance = new Dungeon();
        }
        return instance;
    }


    void Dungeon::dispose(){
        delete instance;
    }

    
    Dungeon::Dungeon(){

    }

    // --== update ==--

    void Dungeon::update(){
        // todo something
        player->update();
        draw();

        updatePlatforms();
        updateEnemies();
        updateDoors();
    }


    // --== drawing ==--

    void Dungeon::draw(){
        
        for (int x = 0; x < currentLoadedLevel.sections[currentLoadedLevel.currentSection].levelData.size(); x++){
            const LevelSlice& slice = currentLoadedLevel.sections[currentLoadedLevel.currentSection].levelData[x];

            for (int y = 0; y < TILES_PER_PATTERN; y++){
                const DungeonTile& tile = dungeonTileLookup[dungeonPatternLookup[slice.geometryPattern].tiles[y]];

                if (dungeonPatternLookup[slice.geometryPattern].tiles[y] != 0){
                    Drawing::get()->drawTexture(tile.sprite, {(float) x * DUNGEON_TILE_SIZE, (float) y * DUNGEON_TILE_SIZE}, false, 1, 0, WHITE, DrawingLayers::LAYER_WORLD);
                }


            }
        }
    }

    // --== camera ==--

    void Dungeon::setCameraPosition(Vector2 playerPosition){
        Vector2 cameraPosition = {playerPosition.x, CAMERA_Y};
        
        cameraPosition.x = std::max(cameraPosition.x, MIN_CAMERA_X);
        cameraPosition.x = std::min(cameraPosition.x, (currentLoadedLevel.sections[currentLoadedLevel.currentSection].sectionLength - 1) * DUNGEON_TILE_SIZE - MIN_CAMERA_X);

        Drawing::get()->getCamera().target = cameraPosition;
    }

    // --== loading ==--
    void Dungeon::enterDungeon(TerrainGeneration::OverworldObject* dungeon){
        // switch state
        State::get()->switchState(GameState::STATE_DUNGEON);

        std::cout << "unload \n";
        unloadDungeon();
        std::cout << "unload done \n";


        if (!dungeon->hasAssignedId){
            dungeon->hasAssignedId = true;
            dungeon->dungeonId = nextDungeonId;
            nextDungeonId++;
        }

        overworldDungeon = dungeon;
        
        

        std::cout << "load \n";

        loadDungeon(overworldDungeon->dungeonId, dungeon->position);

        std::cout << "done \n";

    }


    void Dungeon::unloadDungeon(){
        // TODO
        if (player != nullptr){
            delete player;
            player = nullptr;
        }

        removeAllPlatforms();
    }

    void Dungeon::loadDungeon(int dungeonId, TerrainGeneration::OverworldPosition position){
        if (dungeonId < lastGeneratedDungeonId){
            // TODO something
        }

        currentLoadedLevel = generateDungeon(position);
    }


    // --== generation ==--

    Level Dungeon::generateDungeon(TerrainGeneration::OverworldPosition position){
        
        this->currentSeed = Utils::hashVector(position.x, position.y);
        
        Level output = {
            {},
            0,
            Utils::hashVector(position.x, position.y),
        };

        output.sections.push_back(generateSection(MAIN_SECTION, output, -1, 0, {0.0f,0.0f}));
        

        return output;
    }

    std::vector<int>& Dungeon::getPossibleConnectors(int currentId){
        if (dynamicConnectorLookupTable.find(currentId) != dynamicConnectorLookupTable.end()){
            // found result
            return dynamicConnectorLookupTable.at(currentId);
        }
        std::cout << currentId << " : ";

        // else compute result
        std::vector<int> out;
        const DungeonPattern& currentPattern = dungeonPatternLookup[currentId];

        bool canNextGenerateGap = !currentPattern.generationInfo.isGap && !currentPattern.generationInfo.isWall;
        int heighestNextFloor = currentPattern.generationInfo.floorHeight - TEMPORARY_MAX_JUMP_HEIGHT;

        for (int i = 0; i < dungeonPatternLookup.size(); i++){
            const DungeonPattern& p = dungeonPatternLookup[i];
            
            // no double gaps
            bool canNextBeGap = (!p.generationInfo.isGap || canNextGenerateGap);
            
            // make sure that segments are connected
            // does not apply to walls
            bool connectsToCurrentSegment = 
                (currentPattern.generationInfo.ceilingHeight < p.generationInfo.floorHeight - MIN_TILE_GAP && 
                 currentPattern.generationInfo.floorHeight > p.generationInfo.ceilingHeight + MIN_TILE_GAP) ||
                 currentPattern.generationInfo.isWall;

            // check if jump height is valid
            // does not apply to walls and gaps
            bool isJumpHeightValid = (currentPattern.generationInfo.isWall == true || currentPattern.generationInfo.isGap || 
                                      p.generationInfo.floorHeight >= heighestNextFloor);

            // ✞ bože pomoc ✞
            if (
                !p.generationInfo.isWall && // no walls
                canNextBeGap &&
                connectsToCurrentSegment &&
                isJumpHeightValid
                )
            {
                out.push_back(i);
                std::cout << i << ", ";
            }
        }
        std::cout <<  " \n";


        if (out.size() == 0) {
            std::cout << "neco se posralo pri generaci levelu \n";
            return dynamicConnectorLookupTable[0];
        }

        dynamicConnectorLookupTable[currentId] = out;
        return dynamicConnectorLookupTable.at(currentId);
    }

    

    DungeonSection Dungeon::generateSection(SectionPurpose purpose, Level& level, int returnValue, int sectionId, Vector2 returnLoacation){
        DungeonSection output = {
            {},
            0,
            purpose,
            {},
            {},
            {}
        };

        addPadding(output);
        generateRegularLevel(output, level, -1, 0, returnLoacation);
        addPadding(output);

        output.sectionLength = output.levelData.size();

        return output;
    }
    
    void Dungeon::addPadding(DungeonSection& section){
        for (int i = 0; i < DUNGEON_PADDING; i++){
            section.levelData.push_back({0});
        }
    }

    void Dungeon::generateRegularLevel(DungeonSection& section, Level& level, int returnSection, int sectionId, Vector2 returnLoacation){
        switch (section.purpose) {
            case MAIN_SECTION:
                generateMainSection(section, level, returnSection, sectionId, returnLoacation);
                break;
            case FIGHT_SECTION:
                generateFightSection(section, level, returnSection, sectionId, returnLoacation);
                break;

            case FILLER_SECTION:
                generateFillerSection(section, level, returnSection, sectionId, returnLoacation);
                break;

            case REWARD_SECTION:
                generateRewardSection(section, level, returnSection, sectionId, returnLoacation);
                break;
        }
    }

    void Dungeon::generateMainSection(DungeonSection& section, Level& level, int returnSection, int sectionId, Vector2 returnLoacation){
        int length = Utils::getPseudoRandomInt(MIN_MAIN_SECTION_LENGTH, MAX_MAIN_SECTION_LENGTH, currentSeed++);

        int currentSegment = 0;
        int segmentLength = 0;
        int nextSpawn = 0;
        bool generatedEndDoor = false;
        bool generatedEntrance = false;

        int iterator = 0;
        while (true){
            if (segmentLength == 0){
                // choose next segment
                std::vector<int> connectors = getPossibleConnectors(currentSegment);
                currentSegment = Utils::getPseudoRandomInt(0, connectors.size(), currentSeed++);
                const GenerationInfo& info = dungeonPatternLookup[currentSegment].generationInfo;
                segmentLength = Utils::getPseudoRandomInt(info.minLength, info.maxLength, currentSeed++);
                nextSpawn = Utils::getPseudoRandomInt(0, segmentLength - 1, currentSeed++);
            }
            section.levelData.push_back({currentSegment});


            if (nextSpawn == 0){
                // spawn thing
                const SpawnInfo& info = dungeonPatternLookup[currentSegment].spawnInfo;

                Vector2 position = {(float) (section.levelData.size() * DUNGEON_TILE_SIZE), (float)(TILES_PER_PATTERN - info.spawnY) * DUNGEON_TILE_SIZE};
                EntitySpawnType spawnType = getNextSpawnType(info.spawnType);
                int doorTarget = 0;

                
                // override spawn
                if (spawnType == ENEMY_SPAWN){
                    if (generatedEntrance == false){
                        spawnType = DOOR_SPAWN;
                        doorTarget = returnSection;
                        spawnEntity(position, PLAYER_SPAWN, section, level, doorTarget, returnLoacation);
                        generatedEntrance = true;
                    }
                    
                }
                
                spawnEntity(position, spawnType, section, level, doorTarget, returnLoacation);
            }



            // loop
            segmentLength--;
            nextSpawn--;
            iterator++;
            if (generatedEndDoor){
                break;
            }
            
        }
    }


    void Dungeon::generateRewardSection(DungeonSection& section, Level& level, int returnSection, int sectionId, Vector2 returnLoacation){
        
    }

    

    // --== Entities ==--
    EntitySpawnType Dungeon::getNextSpawnType(SpawnType type){
        switch (type) {
            case NONE:
                return NONE_SPAWN;
            case WATER_FALL_PLATFORM_AND_PLATFORM:
                if (Utils::getPseudoRandomFloat(currentSeed++) > 0.5f){
                    return PLATFORM_SPAWN;
                }
                return WATERFALL_SPAWN;
            case PLATFORM:
                return PLATFORM_SPAWN;
            case GENERIC:
                return ENEMY_SPAWN;
        }
        return NONE_SPAWN;
    }


    void Dungeon::spawnEntity(Vector2 position, EntitySpawnType type, DungeonSection& section, Level& level, int doorTarget, Vector2 returnLoacation){
        switch(type){
            case NONE_SPAWN:
                return;
            case PLAYER_SPAWN:
                if (player == nullptr){
                    player = new DungeonPlayer(position);
                }
                return;
            case ENEMY_SPAWN:
                addEnemy(position, section);
                return;
            case WATERFALL_SPAWN:
                addPlatform(position, true, section.platfromSpawners);
                return;
            case PLATFORM_SPAWN:
                addPlatform(position, false, section.platfromSpawners);
                return;
            case DOOR_SPAWN:
                addDoor(position, doorTarget, section, returnLoacation);
                return;
        }
    }



    // --== Platforms ==--

    void Dungeon::addPlatform(Vector2 position, bool isWaterFall, std::vector<DungeonPlatformSpawner>& platfromSpawners){
        platfromSpawners.push_back({
            {position.x, isWaterFall ? WATERFALL_PLATFORM_SPAWN_HEIGHT : position.y}, isWaterFall, 1
        });
    }

    void Dungeon::updatePlatforms(){
        // update platforms
        for (DungeonPlatform& p : platforms){
            Drawing::get()->drawTexture("platform", p.position, false, 1, 0, WHITE, LAYER_WORLD);
        
            // speed and movement
            p.position.y += p.speed;
            if (!p.isWaterFall && p.isFalling){
                // accelerate
                if (p.speed < MAX_PLATFORM_SPEED){
                    p.speed += PLATFORM_ACCELERATION;
                }
                // set spawner respawn time
                if (p.position.y > PLATFORM_DESPAWN_TRESHOLD){
                    p.spawner->respawnTimer = PLATFORM_RESPAWN_TIME;
                }
            }
        }
        // remove platforms
        platforms.remove_if([this](DungeonPlatform& p){
            return p.position.y > PLATFORM_DESPAWN_TRESHOLD;
        });




        // update spawners
        for (DungeonPlatformSpawner& spawner: currentLoadedLevel.sections[currentLoadedLevel.currentSection].platfromSpawners){
            spawner.respawnTimer -= spawner.respawnTimer >= 0; // goes to -1 to spawn exactly 1 platform
            // spawn new platform
            if (spawner.respawnTimer == 0){
                platforms.push_back(
                    {spawner.position, false, WATERFALL_PLATFORM_SPEED * spawner.isWaterFall, spawner.isWaterFall, &spawner}
                );
                spawner.respawnTimer = WATERFALL_PLATFORM_RESPAWN_TIME * spawner.isWaterFall;
            }

        }
    }


    void Dungeon::removeAllPlatforms(){
        platforms.clear();
    }


    // --== enemies ==--
    void Dungeon::addEnemy(Vector2 position, DungeonSection& section){
        section.enemies.push_back({position});
    }

    void Dungeon::updateEnemies(){
        for (DungeonEnemy& enemy : currentLoadedLevel.sections[currentLoadedLevel.currentSection].enemies){
            Drawing::get()->drawTexture("dungeon_test_3", enemy.position, false, 1, 0, WHITE, LAYER_ENEMY);
        }
    }

    


    // --== collisions ==--
    bool Dungeon::collidesWithDungeon(Vector2 position, Vector2 size){
        return collidesWithDungeon(position, size, true);
    }

    bool Dungeon::collidesWithDungeon(Vector2 position, Vector2 size, bool checkPlatforms){
        return collidesWithLevel(position, size) || (checkPlatforms && collidesWithPlatform(position, size));
    }

    bool Dungeon::collidesWithLevel(Vector2& position, Vector2& size){
        
        DungeonSection& currentSection = currentLoadedLevel.sections[currentLoadedLevel.currentSection];
        
        int xEnd = std::floor(position.x + size.x) / DUNGEON_TILE_SIZE;
        int yEnd = std::floor(position.y + size.y) / DUNGEON_TILE_SIZE;

        for (int x = std::floor(position.x) / DUNGEON_TILE_SIZE; x <= xEnd; x++){
            for (int y = std::floor(position.y) / DUNGEON_TILE_SIZE; y <= yEnd; y++){
                



                if (x < 0 || x > currentSection.sectionLength || y < 0 || y > TILES_PER_PATTERN ||
                    dungeonPatternLookup[currentSection.levelData[x].geometryPattern].tiles[y] != 0){
                    return true;
                }

            }
        }

        return false;
    }

    bool Dungeon::collidesWithPlatformAdvanced(Vector2& position, Vector2& size, Vector2& actualPosition, DungeonPlatform*& platformPointerRef){
        for (DungeonPlatform& p : platforms){
            if (Utils::squaresCollide(position, p.position, size, PLATFORM_SIZE)){
                platformPointerRef = &p;
                return true;
            }
        }
        
        return false;
    }


    bool Dungeon::collidesWithPlatform(Vector2& position, Vector2& size){
        for (DungeonPlatform& p : platforms){
            if (Utils::squaresCollide(position, p.position, size, PLATFORM_SIZE)){
                return true;
            }
        }
        return false;

    }

    bool Dungeon::advancedDungeonCollisions(Vector2 position, Vector2 size, Vector2& actualPosition, DungeonPlatform*& platformPointerRef){
        return collidesWithLevel(position, size) || collidesWithPlatformAdvanced(position, size, actualPosition, platformPointerRef);
    }


    // --== doors ==--
    void Dungeon::addDoor(Vector2 position, int targetSection, DungeonSection& section, Vector2 returnLocation){
        section.doors.push_back({
            position,
            returnLocation,
            targetSection,
        });
    }
    void Dungeon::updateDoors(){
        for (DungeonDoor& door : currentLoadedLevel.sections[currentLoadedLevel.currentSection].doors){
            Drawing::get()->drawTexture("door", {door.position.x, door.position.y - DUNGEON_TILE_SIZE}, false, 1, 0, WHITE, LAYER_OBJECT);

        }
    }

}