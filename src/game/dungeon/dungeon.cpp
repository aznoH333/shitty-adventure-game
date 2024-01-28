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
        int nextY = 0;
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
                        
                        spawnEntity({(float) completeLength * DUNGEON_TILE_SIZE, (float)(TILES_PER_PATTERN - nextY) * DUNGEON_TILE_SIZE}, nextSpawnType);
                    }
                    
                    
                    
                    // segment value update
                    if (currentSegmentLength == 0){
                        // set geometry
                        const DungeonPattern& pattern = dungeonPatternLookup[currentSegmentId];
                        currentSegmentId = pattern.connectors[Utils::getPseudoRandomInt(0, pattern.connectors.size() - 1, currentSeed++)];
                        const DungeonPattern& nextPattern = dungeonPatternLookup[currentSegmentId];
                        currentSegmentLength = Utils::getPseudoRandomInt(nextPattern.minLength, nextPattern.minLength, currentSeed++);
                        
                        // set spawn
                        tryToSpawnExit = generationLength > desiredLength && nextPattern.spawnInfo.spawnType == GENERIC;
                        nextSpawnType = getNextSpawnType(nextPattern.spawnInfo.spawnType);
                        nextY = nextPattern.spawnInfo.spawnY + 1;
                        nextSpawnDist = Utils::getPseudoRandomInt(0, currentSegmentLength, currentSeed++);
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

                break;

            case WATERFALL_SPAWN:
                std::cout << "spawned waterfall at " << position.x << ", " << position.y << "\n";

                break;

            case ENEMY_SPAWN:
                std::cout << "spawned enemy at " << position.x << ", " << position.y << "\n";

                break;

            case NONE_SPAWN:
                break; // do nothing
        }
    }

}