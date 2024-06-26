#include "game/dungeon/dungeon.h"
#include "game/state.h"
#include <iostream>
#include "game/dungeon/enemyManager.h"

namespace DungeonCode {
    using namespace Utils;

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
        if (mojeNoha != nullptr){
            if (mojeNoha->target == -1){
                exitDungeon();
                mojeNoha = nullptr;
                return;
            }
            delete player;

            clearTemporaryObjects();
            currentLoadedLevel.currentSection = mojeNoha->target;
            Vector2 position = mojeNoha->useDefaultEntry ? currentLoadedLevel.sections.at(currentLoadedLevel.currentSection).defaultEntry : mojeNoha->exitLocation;
            player = new DungeonPlayer(position);
            mojeNoha = nullptr;
            return;
        }
        
        DungeonSection& section = currentLoadedLevel.sections.at(currentLoadedLevel.currentSection);
        draw(section);

        updatePlatforms(section);
        updateEnemies(section);
        updateDoors(section);
        updateParticles();
        updateProjectiles();
        updateBoxes();
        player->update();

    }

    void Dungeon::clearTemporaryObjects(){
        removeAllPlatforms();
        clearAllProjectiles();
    }


    // --== drawing ==--
    void Dungeon::draw(DungeonSection& section){
        section.background.draw(Drawing::get()->getCamera().target.x);
        for (int x = 0; x < section.levelData.size(); x++){
            const LevelSlice& slice = section.levelData[x];

            for (int y = 0; y < TILES_PER_PATTERN; y++){
                const DungeonTile& tile = theme->getTile(slice.tiles[y]);

                if (slice.tiles[y] != 0){
                    Drawing::get()->drawTexture(tile.sprite, {(float) x * DUNGEON_TILE_SIZE, (float) y * DUNGEON_TILE_SIZE}, false, 1, 0, WHITE, DrawingLayers::LAYER_WORLD);
                }


            }
        }
    }

    // --== camera ==--

    void Dungeon::setCameraPosition(Vector2 playerPosition){
        Vector2 cameraPosition = {playerPosition.x, CAMERA_Y};
        
        cameraPosition.x = std::max(cameraPosition.x, MIN_CAMERA_X);
        cameraPosition.x = std::min(cameraPosition.x, (currentLoadedLevel.sections.at(currentLoadedLevel.currentSection).sectionLength - 1) * DUNGEON_TILE_SIZE - MIN_CAMERA_X);

        Drawing::get()->getCamera().target = cameraPosition;
    }

    // --== loading ==--
    void Dungeon::enterDungeon(TerrainGeneration::OverworldObject* dungeon, float distanceToSpawn){
        // switch state
        State::get()->switchState(GameState::STATE_DUNGEON);

        unloadDungeon();

        calculateDificulity(distanceToSpawn);

        if (!dungeon->hasAssignedId){
            dungeon->hasAssignedId = true;
            dungeon->dungeonId = nextDungeonId;
            nextDungeonId++;
        }

        overworldDungeon = dungeon;
        theme = dungeon->theme;
        loadDungeon(overworldDungeon->dungeonId, dungeon->position);

        UICode::Hud::setHudVisibility(true);
    }


    void Dungeon::calculateDificulity(float distanceToSpawn){
        dungeonDificulity = std::min(MIN_DIFICULITY + (distanceToSpawn / DIFICULITY_DIVIDER), MAX_DIFICULITY);
        enemySpawnChance = (dungeonDificulity / MAX_DIFICULITY) * 0.5f + 0.3f;
    }


    void Dungeon::exitDungeon(){
        unloadDungeon();
        UICode::Hud::setHudVisibility(false);
        State::get()->switchState(GameState::STATE_OVERWORLD);
        
    }


    void Dungeon::unloadDungeon(){
        // TODO
        if (player != nullptr){
            delete player;
            player = nullptr;
        }

        clearTemporaryObjects();
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
            0
        };

        output.sections.insert({0,generateSection({MAIN_SECTION, true}, output, -1, 0, {0.0f,0.0f})});
        

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

    

    DungeonSection Dungeon::generateSection(SectionInfo info, Level& level, int returnValue, int sectionId, Vector2 returnLoacation){
        DungeonSection output = {
            {},
            0,
            info,
            {},
            {},
            {},
            {},
            {},
            DungeonBackground::generateBasedOnTheme(theme->getId())
        };
        
        
        
        addPadding(output);
        generateRegularLevel(output, level, returnValue, sectionId, returnLoacation);
        addPadding(output);

        output.sectionLength = output.levelData.size();

        return output;
    }
    
    void Dungeon::addPadding(DungeonSection& section){
        for (int i = 0; i < DUNGEON_PADDING; i++){
            section.levelData.push_back(generateSliceBasedOnPattern(0));
        }
    }

    void Dungeon::generateRegularLevel(DungeonSection& section, Level& level, int returnSection, int sectionId, Vector2 returnLoacation){
        switch (section.sectionInfo.purpose) {
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


    LevelSlice Dungeon::generateSliceBasedOnPattern(int patternId){
        LevelSlice l = {};
        l.geometryPattern = patternId;
        const DungeonPattern& pattern = dungeonPatternLookup[patternId];
        for (int i = 0; i < TILES_PER_PATTERN; i++){
            l.tiles[i] = theme->translateTileId(pattern.tiles[i]);
        }

        return l;
    }

    void Dungeon::generateGeneric(DungeonSection& section, Level& level, int returnSection, int sectionId, Vector2 returnLoacation, bool isMain){
        
        
        int length;

        if (isMain){
            length = Utils::getPseudoRandomInt(MIN_MAIN_SECTION_LENGTH, MAX_MAIN_SECTION_LENGTH, currentSeed++);
        }else {
            length = Utils::getPseudoRandomInt(MIN_SECTION_LENGTH, MAX_SECTION_LENGTH, currentSeed++);
        }

        int currentSegment = 0;
        int segmentLength = 0;
        int nextSpawn = 0;
        bool generatedEndDoor = false;
        bool generatedEntrance = false;
        EntitySpawnType nextSpawnType = NONE_SPAWN;

        int iterator = 0;
        while (true){
            if (segmentLength == 0){
                // choose next segment
                std::vector<int> connectors = getPossibleConnectors(currentSegment);
                currentSegment = connectors[Utils::getPseudoRandomInt(0, connectors.size() - 1, currentSeed++)];
                const GenerationInfo& info = dungeonPatternLookup[currentSegment].generationInfo;
                segmentLength = Utils::getPseudoRandomInt(info.minLength, info.maxLength, currentSeed++);
                nextSpawnType = getNextSpawnType(dungeonPatternLookup[currentSegment].spawnInfo.spawnType);
                nextSpawn = (nextSpawnType == PLATFORM_SPAWN || nextSpawnType == WATERFALL_SPAWN ? 1 : Utils::getPseudoRandomInt(0, segmentLength - 1, currentSeed++));
                
                // break loop
                if (generatedEndDoor){
                    break;
                }
            }
            section.levelData.push_back(generateSliceBasedOnPattern(currentSegment));


            if (nextSpawn == 0){
                // spawn thing
                const SpawnInfo& info = dungeonPatternLookup[currentSegment].spawnInfo;

                Vector2 position = {(float) ((section.levelData.size() - 1) * DUNGEON_TILE_SIZE), (float)(TILES_PER_PATTERN - info.spawnY - 1) * DUNGEON_TILE_SIZE};
                int doorTarget = 0;
                bool useDefaultEnrty = true;
                

                // override spawn
                if (nextSpawnType == ENEMY_SPAWN){
                    float rng = Utils::getPseudoRandomFloat(currentSeed++);
                    
                    if (generatedEntrance == false){
                        // spawn entry door
                        nextSpawnType = DOOR_SPAWN;
                        doorTarget = returnSection;
                        useDefaultEnrty = false;
                        spawnEntity(position, PLAYER_SPAWN, section, level, doorTarget, returnLoacation, false);
                        generatedEntrance = true;
                        section.defaultEntry = position;
                    }else if (iterator > length && generatedEndDoor == false){
                        // spawn exit door
                        nextSpawnType = DOOR_SPAWN;
                        useDefaultEnrty = true;
                        int nextId = ++level.lastId;
                        doorTarget = nextId;
                        SectionInfo nextInfo = getNextPurpose(section.sectionInfo);
                        
                        level.sections.insert({nextId, generateSection(nextInfo, level, sectionId, nextId, position)});
                        generatedEndDoor = true;
                    }else if (rng < SIDE_ROOM_CHANCE){
                        // spawn side door
                        nextSpawnType = DOOR_SPAWN;
                        useDefaultEnrty = true;
                        int nextId = ++level.lastId;
                        doorTarget = nextId;
                        SectionInfo nextInfo = getNextPurpose({section.sectionInfo.purpose, false});
                        
                        level.sections.insert({nextId, generateSection(nextInfo, level, sectionId, nextId, position)});
                    }
                }
                
                spawnEntity(position, nextSpawnType, section, level, doorTarget, returnLoacation, useDefaultEnrty);
            }



            // loop
            segmentLength--;
            nextSpawn--;
            iterator++;
        }
    }


    void Dungeon::generateMainSection(DungeonSection& section, Level& level, int returnSection, int sectionId, Vector2 returnLoacation){
        generateGeneric(section, level, returnSection, sectionId, returnLoacation, true);
    }

    SectionInfo Dungeon::getNextPurpose(SectionInfo info){
        switch (info.purpose){
            case MAIN_SECTION:
                if (Utils::getPseudoRandomFloat(currentSeed++) > 0.5){
                    return {FIGHT_SECTION, info.isMain};
                }
                return {FILLER_SECTION, info.isMain};
            case FILLER_SECTION:
                if (Utils::getPseudoRandomFloat(currentSeed++) > 0.5){
                    return {FIGHT_SECTION, info.isMain};
                }
                return {REWARD_SECTION, info.isMain};
            case FIGHT_SECTION:
                return {REWARD_SECTION, info.isMain};
            default:
                return {MAIN_SECTION, false};
        }
    }


    void Dungeon::generateRewardSection(DungeonSection& section, Level& level, int returnSection, int sectionId, Vector2 returnLoacation){
        
        int size = section.sectionInfo.isMain ? REWARD_ROOM_SIZE : SMALL_REWARD_ROOM_SIZE;
        
        
        
        for (int i = 0; i < size; i++){

            int distFromCenter = std::abs(i - (size >> 1));
            int patternId;


            if (distFromCenter > 9){
                patternId = 8;
            }else if (distFromCenter > 7){
                patternId = 11;
            }else {
                patternId = 12;
            }
            
            section.levelData.push_back(generateSliceBasedOnPattern(patternId));
            const SpawnInfo& info = dungeonPatternLookup[patternId].spawnInfo;
            Vector2 position = {(float) ((section.levelData.size() - 1) * DUNGEON_TILE_SIZE), (float)(TILES_PER_PATTERN - info.spawnY - 1) * DUNGEON_TILE_SIZE};

            if (i == 1){
                // spawn door
                spawnEntity(position, DOOR_SPAWN, section, level, returnSection, returnLoacation, false);
                // set default position
                section.defaultEntry = position;
            }else if (i == size / 2){
                // spawn box
                int itemId = PlayerStats::ItemManager::get()->generateNewItem(currentSeed++, dungeonDificulity);
                section.boxes.push_back(ItemBox({position.x - DUNGEON_TILE_SIZE, position.y - DUNGEON_TILE_SIZE}, itemId));
            }
        }
    }


    void Dungeon::generateFightSection(DungeonSection& section, Level& level, int returnSection, int sectionId, Vector2 returnLoacation){
        for (int i = 0; i < FIGHT_ROOM_SIZE; i++){

            int patternId = 8;

            section.levelData.push_back(generateSliceBasedOnPattern(patternId));


            if (i == 1){
                const SpawnInfo& info = dungeonPatternLookup[patternId].spawnInfo;
                
                Vector2 position = {(float) ((section.levelData.size() - 1) * DUNGEON_TILE_SIZE), (float)(TILES_PER_PATTERN - info.spawnY - 1) * DUNGEON_TILE_SIZE};
                
                // spawn door
                spawnEntity(position, DOOR_SPAWN, section, level, returnSection, returnLoacation, false);
                // set default position
                section.defaultEntry = position;
            }else if (i == FIGHT_ROOM_SIZE - 2){
                const SpawnInfo& info = dungeonPatternLookup[patternId].spawnInfo;
                
                Vector2 position = {(float) ((section.levelData.size() - 1) * DUNGEON_TILE_SIZE), (float)(TILES_PER_PATTERN - info.spawnY - 1) * DUNGEON_TILE_SIZE};
                
                // spawn door
                spawnEntity(position, DOOR_SPAWN, section, level, sectionId, position, true);

                // spawn side door
                int nextId = ++level.lastId;
                SectionInfo nextInfo = getNextPurpose(section.sectionInfo);
                level.sections.insert({nextId, generateSection(nextInfo, level, sectionId, nextId, position)});
            
                spawnEntity(position, DOOR_SPAWN, section, level, nextId, returnLoacation, true);

            }
        }
    }

    void Dungeon::generateFillerSection(DungeonSection& section, Level& level, int returnSection, int sectionId, Vector2 returnLoacation){
        generateGeneric(section, level, returnSection, sectionId, returnLoacation, false);
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


    void Dungeon::spawnEntity(Vector2 position, EntitySpawnType type, DungeonSection& section, Level& level, int doorTarget, Vector2 returnLoacation, bool useDefaultEntry){
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
                addDoor(position, doorTarget, section, returnLoacation, useDefaultEntry);
                return;
        }
    }



    // --== Platforms ==--

    void Dungeon::addPlatform(Vector2 position, bool isWaterFall, std::vector<DungeonPlatformSpawner>& platfromSpawners){
        platfromSpawners.push_back({
            {position.x, isWaterFall ? WATERFALL_PLATFORM_SPAWN_HEIGHT : position.y}, isWaterFall, 1
        });
    }

    void Dungeon::updatePlatforms(DungeonSection& section){
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
        for (DungeonPlatformSpawner& spawner: section.platfromSpawners){
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

    // --== misc ==--
    DungeonPlayer* Dungeon::getPlayer(){
        return player;
    }


    // --== enemies ==--
    void Dungeon::addEnemy(Vector2 position, DungeonSection& section){
        if (getPseudoRandomFloat(currentSeed++) < enemySpawnChance){
            section.enemies.push_back(EnemyManager::get()->initEnemy(position, std::ceil(dungeonDificulity) + getPseudoRandomFloat(0,2, currentSeed++), currentSeed));
        }
    }

    void Dungeon::updateEnemies(DungeonSection& section){
        
        section.enemies.remove_if([this](DungeonEnemy& enemy){
            enemy.update();
            Vector2 pos = enemy.getPosition();
            
            float size = enemy.getSize();

            pos.x -= 8.0f - ((1.0f - size) * 32.0f / 2);
            pos.y += (1.0f - size) * DungeonEnemy::ENEMY_SIZE.y;
            Drawing::get()->drawTexture(enemy.getSprite(), pos, enemy.getFlip(), size, 0, enemy.getColor(), LAYER_ENEMY);

            

            for (Projectile& p : projectiles){
                if (p.isAlliedWithPlayer() && p.canDamage() == true && Utils::squaresCollide(enemy.getPosition(), p.getPosition(), DungeonEnemy::ENEMY_SIZE, PROJECTILE_SIZE)){
                    p.resetPierceTimer();
                    enemy.takeDamage(p.getDamage());
                    enemy.lastHitDirection = p.getRotation();
                }
            }

            // player collisions
            if (squaresCollide(enemy.getPosition(), player->getPosition(), DungeonEnemy::ENEMY_SIZE, DungeonPlayer::SIZE)){
                player->takeDamage(enemy.getContactDamage());
            }

            if (enemy.health <= 0.0f){
                // spawn giblets
                for (int i = 0; i < 10; i++){
                    float gibletSpeed = Utils::getRandomFloat(1,2.5f);
                    float direction = enemy.lastHitDirection * DEG2RAD + Utils::getRandomFloat(-0.2f, 0.2f);
                    
                    ParticleBuilder* builder = new ParticleBuilder(enemy.getPosition(), concatSprite("giblet_", GetRandomValue(1, 3)), GetRandomValue(20, 60));
                    builder->setGravity({0, 0.3f});
                    builder->setVelocity({std::cos(direction) * gibletSpeed + Utils::getRandomFloat(-2.5,2.2), std::sin(direction) * gibletSpeed + Utils::getRandomFloat(-2.5,2.2)});
                    addParticle(builder);
                }
                enemy.dispose();

                return true;
            }
            return false;
        });
    }

    


    // --== collisions ==--
    bool Dungeon::collidesWithDungeon(Vector2 position, Vector2 size){
        return collidesWithDungeon(position, size, true);
    }

    bool Dungeon::collidesWithDungeon(Vector2 position, Vector2 size, bool checkPlatforms){
        return collidesWithLevel(position, size) || (checkPlatforms && collidesWithPlatform(position, size));
    }

    bool Dungeon::collidesWithLevel(Vector2& position, Vector2& size){
        return lambdaTileCheck(position, size, [](int tileId){return tileId != 0;});
    }

    bool Dungeon::lambdaTileCheck(Vector2& position, Vector2& size, std::function<bool(int)> tileCheckFunction){
        DungeonSection& currentSection = currentLoadedLevel.sections.at(currentLoadedLevel.currentSection);
        
        int xEnd = std::floor(position.x + size.x) / DUNGEON_TILE_SIZE;
        int yEnd = std::floor(position.y + size.y) / DUNGEON_TILE_SIZE;

        for (int x = std::floor(position.x) / DUNGEON_TILE_SIZE; x <= xEnd; x++){
            for (int y = std::floor(position.y) / DUNGEON_TILE_SIZE; y <= yEnd; y++){
                



                if (x < 0 || x > currentSection.sectionLength || y < 0 || y >= TILES_PER_PATTERN ||
                    tileCheckFunction(dungeonPatternLookup[currentSection.levelData[x].geometryPattern].tiles[y])){
                    return true;
                }

            }
        }

        return false;
    }


    bool Dungeon::collidesWithSpikes(Vector2 position, Vector2 size){
        return lambdaTileCheck(position, size, 
        [](int tileId){
            return dungeonTileLookup[tileId].isSpike;
        });
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
    void Dungeon::addDoor(Vector2 position, int targetSection, DungeonSection& section, Vector2 returnLocation, bool useDefaultEntry){
        section.doors.push_back({
            position,
            returnLocation,
            targetSection,
            useDefaultEntry,
        });
    }
    void Dungeon::updateDoors(DungeonSection& section){
        Vector2& playerPos = player->getPosition();
        
        player->setNearbyDoor(nullptr);

        for (DungeonDoor& door : section.doors){
            
            
            // check player proximity
            float distToPlayer = Utils::pythagoras(door.position.x, door.position.y, playerPos.x, playerPos.y);
            
            Drawing::get()->drawTexture("door", {door.position.x, door.position.y - DUNGEON_TILE_SIZE}, false, 1, 0, WHITE, LAYER_OBJECT);

            if (distToPlayer < NEARBY_DOOR_DISTANCE){
                player->setNearbyDoor(&door);
            }

        }
    }

    // holy shit
    // uz vim proc se rika ze si ustrelis nohu
    void Dungeon::enterDoor(DungeonDoor* door){
        mojeNoha = door;
    }


    // --== particles ==--
    void Dungeon::addParticle(ParticleBuilder* builder){
        particles.push_back(Particle(builder));
    }

    void Dungeon::updateParticles(){
        
        Drawing* d = Drawing::get();
        
        particles.remove_if([this, d](Particle& p){
            p.update();

            if (p.shouldBeDestroyed() || p.getPosition().y > TILES_PER_PATTERN * DUNGEON_TILE_SIZE){
                p.dispose();
                return true;
            }
            return false;
        });
    }
    void Dungeon::clearAllParticles(){
        for (Particle p : particles){
            p.dispose();
        }
        particles.clear();
    }


    // --== projectiles ==--
    void Dungeon::addProjectile(Projectile projectile){
        projectiles.push_back(projectile);
    }

    void Dungeon::updateProjectiles(){
        
        Drawing* d = Drawing::get();
        
        projectiles.remove_if([this, d](Projectile& p){
            if (p.isAlliedWithPlayer() == false && p.canDamage() && squaresCollide(player->getPosition(), p.getPosition(), DungeonPlayer::SIZE, PROJECTILE_SIZE)){
                player->takeDamage(p.getDamage());
                p.resetPierceTimer();
            }
            p.update();
            return p.shouldDestroy();
        });
    }
    void Dungeon::clearAllProjectiles(){
        projectiles.clear();
    }

    // --== boxes ==--
    void Dungeon::updateBoxes(){
        for (ItemBox& box : currentLoadedLevel.sections.at(currentLoadedLevel.currentSection).boxes){
            box.update();

            for (Projectile& p : projectiles){
                if (!box.isBoxOpen() && squaresCollide(box.getPosition(), p.getPosition(), BOX_SIZE, PROJECTILE_SIZE)){
                    box.openBox();
                }
            }
        }
    }

    ItemBox* Dungeon::lookForNearbyBoxes(Vector2 position){
        for (ItemBox& box : currentLoadedLevel.sections.at(currentLoadedLevel.currentSection).boxes){
            float distance = pythagoras(position.x, position.y, box.getPosition().x, box.getPosition().y);

            if (distance < MAX_DISTANCE_TO_BOX && box.isBoxOpen()){
                return &box;
            }
        }
        return nullptr;
    }
}