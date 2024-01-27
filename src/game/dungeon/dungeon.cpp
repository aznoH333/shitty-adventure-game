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
        
        // set vals
        currentSegmentId = 0;
        remainingSegmantLength = 0;
        nextSpawnIndex = 0;
        nextSpawnType = NONE_SPAWN; 
        playerSpawned = false;

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



    LevelSlice Dungeon::getNextSegment(int index, const DungeonSection* level){
        // level padding
        if (index < DUNGEON_PADDING || index > level->sectionLength - DUNGEON_PADDING){
            return {0}; // return full wall
        }
            
            
        if (remainingSegmantLength == 0 || currentSegmentId == 0){
            // choose next segment
            const std::vector<int>& connector = dungeonPatternLookup[currentSegmentId].connectors;

            int rng = Utils::getPseudoRandomInt(0, connector.size() - 1, currentSeed++);
            currentSegmentId = connector[rng];

            // set length
            const DungeonPattern& p = dungeonPatternLookup[currentSegmentId];
            remainingSegmantLength = Utils::getPseudoRandomInt(p.minLength, p.maxLength, currentSeed++);

            // next spawn logic
            nextSpawnIndex = Utils::getPseudoRandomInt(0, p.maxLength, currentSeed++);
            nextSpawnType = getNextSpawnType(p.spawnInfo.spawnType);
            nextSpawnY = p.spawnInfo.spawnY + 1;
            
            

        }
        
        // return regular
        remainingSegmantLength--;
        nextSpawnIndex--;
        return {currentSegmentId};

            
            
    }


    DungeonSection Dungeon::generateSection(SectionPurpose purpose){
        DungeonSection output;
        // set length
        if (purpose == SectionPurpose::MAIN_SECTION){
            output.sectionLength = Utils::getPseudoRandomInt(MIN_MAIN_SECTION_LENGTH, MAX_MAIN_SECTION_LENGTH, currentSeed++);
        }else {
            output.sectionLength = Utils::getPseudoRandomInt(MIN_SECTION_LENGTH, MAX_SECTION_LENGTH, currentSeed++);
        }

        
        for (int i = 0; i < output.sectionLength; i++){
            if (i > output.sectionLength - DUNGEON_PADDING && remainingSegmantLength != 0){
                output.sectionLength++;
            }

            if (nextSpawnIndex == 0){
                spawnEntity({(float)(i * DUNGEON_TILE_SIZE), (float)((TILES_PER_PATTERN - nextSpawnY) * DUNGEON_TILE_SIZE)}, nextSpawnType);
            }

            output.levelData.push_back(getNextSegment(i, &output));
            
            
        }

        return output;
    }


    // --== Entities ==--
    EntitySpawnType Dungeon::getNextSpawnType(SpawnType type){
        if (playerSpawned == false){
            playerSpawned = true;
            return PLAYER_SPAWN;
        }
                
        switch(type){
            case NONE:
                return NONE_SPAWN;

            case WATER_FALL_PLATFORM_AND_PLATFORM:
                if (Utils::getPseudoRandomInt(0, 1, currentSeed++) > 0){
                    return WATERFALL_SPAWN;
                }
                return PLATFORM_SPAWN;

            case PLATFORM_SPAWN:
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
        }
    }

}