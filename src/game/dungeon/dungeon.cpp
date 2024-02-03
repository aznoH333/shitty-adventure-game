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
        removeAllEnemies();
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

        output.sections.push_back(generateSection(MAIN_SECTION));

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

        bool canNextBeGap = !currentPattern.generationInfo.isGap;

        for (int i = 0; i < dungeonPatternLookup.size(); i++){
            const DungeonPattern& p = dungeonPatternLookup[i];
            

            // ✞ bože pomoc ✞
            if (
                
                // no walls
                !p.generationInfo.isWall &&
                // no double gaps
                (!p.generationInfo.isGap || canNextBeGap) &&

                // make sure that segments are connected
                // does not apply to walls
                (currentPattern.generationInfo.isWall 
                || 
                (currentPattern.generationInfo.ceilingHeight < p.generationInfo.floorHeight - MIN_TILE_GAP && 
                currentPattern.generationInfo.floorHeight > p.generationInfo.ceilingHeight + MIN_TILE_GAP)
                )



                
                
                ){
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

    


    DungeonSection Dungeon::generateSection(SectionPurpose purpose){
        DungeonSection output;
        int desiredLength;
        
        // set length
        if (purpose == SectionPurpose::MAIN_SECTION){
            desiredLength = Utils::getPseudoRandomInt(MIN_MAIN_SECTION_LENGTH, MAX_MAIN_SECTION_LENGTH, currentSeed++);
        }else {
            desiredLength = Utils::getPseudoRandomInt(MIN_SECTION_LENGTH, MAX_SECTION_LENGTH, currentSeed++);
        }

        // controll vals
        int generationState = 0;
        bool isGenerationDone = false;
        int generationLength = 0;
        int completeLength = 0;


        // generation vals
        int currentSegmentLength = 0;
        int currentSegmentId = 0;
        EntitySpawnType nextSpawnType = NONE_SPAWN;
        int nextSpawnDist = 0;
        int nextSpawnY = 0;
        bool spawnedPlayer = false;
        bool spawnedExit = false;
        bool tryToSpawnExit = false;



        while (!isGenerationDone){
            switch (generationState){
                case 0:
                case 2:
                    // padding wall
                    output.levelData.push_back({0}); 

                    if (generationLength == DUNGEON_PADDING){
                        // exit state
                        generationState++;
                        generationLength = 0;

                    }
                    break;

                case 1:
                    
                    
                    
                    
                    // segment value update
                    if (currentSegmentLength == 0){
                        // set geometry
                        const DungeonPattern& pattern = dungeonPatternLookup[currentSegmentId];
                        
                        std::vector<int>& possibleConnectors = getPossibleConnectors(currentSegmentId);


                        

                        currentSegmentId = possibleConnectors[Utils::getPseudoRandomInt(0, possibleConnectors.size() - 1, currentSeed++)];
                        const DungeonPattern& nextPattern = dungeonPatternLookup[currentSegmentId];
                        currentSegmentLength = Utils::getPseudoRandomInt(nextPattern.generationInfo.minLength, nextPattern.generationInfo.maxLength, currentSeed++);
                        
                        // set spawn
                        tryToSpawnExit = generationLength > desiredLength && nextPattern.spawnInfo.spawnType == GENERIC;
                        nextSpawnType = getNextSpawnType(nextPattern.spawnInfo.spawnType);
                        nextSpawnY = nextPattern.spawnInfo.spawnY + 1;
                        std::cout << "seg id " << currentSegmentId << "\n"; 

                        nextSpawnDist = Utils::getPseudoRandomInt(0, currentSegmentLength - 1, currentSeed++);

                        // exception for platforms
                        if (nextSpawnType == PLATFORM_SPAWN || nextSpawnType == WATERFALL_SPAWN){
                            nextSpawnDist = 1;
                        }
                    }

                    // spawn entity
                    if (nextSpawnDist == 0){
                        // special cases
                        if (spawnedPlayer == false){
                            spawnedPlayer = true;
                            nextSpawnType = PLAYER_SPAWN;
                        }else if (tryToSpawnExit){
                            spawnedExit = true;
                            nextSpawnType = DOOR_SPAWN; // TODO
                        }
                        
                        spawnEntity({(float) completeLength * DUNGEON_TILE_SIZE, (float)(TILES_PER_PATTERN - nextSpawnY) * DUNGEON_TILE_SIZE}, nextSpawnType);
                    }
                    

                    // add geometry
                    output.levelData.push_back({currentSegmentId});
                    currentSegmentLength--;
                    nextSpawnDist--;

                    
                    // exit status
                    if (generationLength > desiredLength && currentSegmentLength == 0 && spawnedExit){
                        generationState++;
                        generationLength = 0;
                    }
                    break;
                case 3:
                    // generation done
                    isGenerationDone = true;
                    break;
            }


            generationLength++;
            completeLength++;
        }
        
        output.sectionLength = completeLength;


        return output;
    }


    // --== Entities ==--
    EntitySpawnType Dungeon::getNextSpawnType(SpawnType type){        
        switch(type){
            case NONE:
                return NONE_SPAWN;

            case WATER_FALL_PLATFORM_AND_PLATFORM:
                if (Utils::getPseudoRandomInt(0, 1, currentSeed++) > 0){
                    return WATERFALL_SPAWN;
                }
                return PLATFORM_SPAWN;

            case PLATFORM:
                return PLATFORM_SPAWN;
            default:
            case GENERIC:
                return ENEMY_SPAWN;// TODO
        }
        
    }

    void Dungeon::spawnEntity(Vector2 position, EntitySpawnType spawnType){
        switch (spawnType){
            case PLAYER_SPAWN:
                if (player != nullptr){
                    delete player;
                }
                player = new DungeonPlayer(position);
                std::cout << "spawned player at " << position.x << ", " << position.y << "\n";
                break;

            case DOOR_SPAWN:
                std::cout << "spawned door at " << position.x << ", " << position.y << "\n";

                break;

            case ITEM_SPAWN:
                std::cout << "spawned item at " << position.x << ", " << position.y << "\n";

                break;

            case PLATFORM_SPAWN:
                std::cout << "spawned platform at " << position.x << ", " << position.y << "\n";
                addPlatform(position, false);
                break;

            case WATERFALL_SPAWN:
                std::cout << "spawned waterfall at " << position.x << ", " << position.y << "\n";
                addPlatform(position, true);
                break;

            case ENEMY_SPAWN:
                std::cout << "spawned enemy at " << position.x << ", " << position.y << "\n";
                addEnemy(position);
                break;

            case NONE_SPAWN:
                break; // do nothing
        }
    }



    // --== Platforms ==--

    void Dungeon::addPlatform(Vector2 position, bool isWaterFall){
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
        for (DungeonPlatformSpawner& spawner: platfromSpawners){
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
        platfromSpawners.clear();
        platforms.clear();
    }


    // --== enemies ==--
    void Dungeon::addEnemy(Vector2 position){
        enemies.push_back({position});
    }

    void Dungeon::updateEnemies(){
        for (DungeonEnemy& enemy : enemies){
            Drawing::get()->drawTexture("dungeon_test_3", enemy.position, false, 1, 0, WHITE, LAYER_ENEMY);
        }
    }

    void Dungeon::removeAllEnemies(){
        enemies.clear();
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
}