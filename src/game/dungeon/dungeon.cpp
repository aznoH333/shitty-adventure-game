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
        for (const LevelSlice& slice : currentLoadedLevel.sections[currentLoadedLevel.currentSection].levelData){
            for (const int tileId : dungeonPatternLookup[slice.geometryPattern]){
                const DungeonTile& tile = dungeonTileLookup[tileId];


            }
        }

        for (int x = 0; x < currentLoadedLevel.sections[currentLoadedLevel.currentSection].levelData.size(); x++){
            const LevelSlice& slice = currentLoadedLevel.sections[currentLoadedLevel.currentSection].levelData[x];

            for (int y = 0; y < TILES_PER_PATTERN; y++){
                const DungeonTile& tile = dungeonTileLookup[dungeonPatternLookup[slice.geometryPattern][y]];

                if (dungeonPatternLookup[slice.geometryPattern][y] != 0){
                    Drawing::get()->drawTexture(tile.sprite, {(float) x * DUNGEON_TILE_SIZE, (float) y * DUNGEON_TILE_SIZE}, false, 1, 0, WHITE, DrawingLayers::LAYER_WORLD);
                }


            }
        }
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
        // spawn player
        player = new DungeonPlayer({0,0});

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
        // set length
        if (purpose == SectionPurpose::MAIN_SECTION){
            output.sectionLength = Utils::getPseudoRandomInt(MIN_MAIN_SECTION_LENGTH, MAX_MAIN_SECTION_LENGTH, currentSeed++);
        }else {
            output.sectionLength = Utils::getPseudoRandomInt(MIN_SECTION_LENGTH, MAX_SECTION_LENGTH, currentSeed++);
        }

        
        for (int i = 0; i < output.sectionLength; i++){
            // fill with walls
            if (i < DUNGEON_PADDING || i > output.sectionLength - DUNGEON_PADDING){
                output.levelData.push_back({0});
            }else {
                // temporary garbage generation
                output.levelData.push_back({Utils::getPseudoRandomInt(1, 5, currentSeed++)});
            }
        }

        return output;
    }

}