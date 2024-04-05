#include "game/overworld/overworldTerrain.h"
    #include <iostream>

using namespace Utils;
namespace TerrainGeneration {


    OverworldTerrain::OverworldTerrain(){
        worldLoadingThread = std::thread(&OverworldTerrain::worldLoadingFunction, this);
        

    }

    void OverworldTerrain::init(int seed){
        
        noiseMap = new ValueNoiseMap(seed);
        mountainNoiseMap = new SpotNoiseMap(seed);
        treeNoiseMap = new ValueNoiseMap(seed);
        biomeMap = new BiomeNoiseMap(seed, BIOME_SIZE);
        structureNoiseMap = new SpotNoiseMap(seed);
        /*
        // init some garbage data
        for (int i = 0; i < DISPLAYED_CHUNK_COUNT; i++){
            displayedChunks[i] = generateChunk({i % 3,i/3});
            
        }*/
    }

    OverworldTerrain::~OverworldTerrain(){
        // stop thread
        shouldWorldLoadingThreadBeRunning = false;
        worldLoadingThread.join();
        
        
        // free loaded chunks
        for (const auto chunk : loadedChunks){
            delete chunk.second;
        }

        delete noiseMap;
        delete mountainNoiseMap;
        delete treeNoiseMap;
        delete structureNoiseMap;
        delete biomeMap;
    }



    // --== Update ==--
    void OverworldTerrain::update(){
        
        draw();
    }

    void OverworldTerrain::chunkLoadingUpdate(){
        if (Utils::gameTickCounter % DEFAULT_WORLD_LOADING_INTERVAL == 0 || initialLoading){
            shouldAttemptLoading = true;
        }
    }


    // --== collisions ==--

    bool OverworldTerrain::isTileBlocking(ChunkCoordinates chunkPosition, int x, int y){
        
        return tileLookupTable[loadedChunks[chunkPosition]->tiles[x][y].tileId].blocksMovement;

    }

    

    // --== chunk loading ==--
    bool OverworldTerrain::isDoneLoading(){
        return !initialLoading;
    }
    
    
    void OverworldTerrain::worldLoadingFunction(){
        while (shouldWorldLoadingThreadBeRunning){
            if (shouldAttemptLoading && !allChunksLoaded){
                shouldAttemptLoading = false;
                generateChunksAroundPoint(lastLoadChunk);
            }else {
                std::this_thread::sleep_for(std::chrono::milliseconds(DEFAULT_THREAD_WAIT));
            }
        }   
    }
    
    
    void OverworldTerrain::generateChunksAroundPoint(ChunkCoordinates coordinates){
        for (int x = -CHUNK_GENERATION_DISTANCE; x <= CHUNK_GENERATION_DISTANCE; x++){
            for (int y = -CHUNK_GENERATION_DISTANCE; y <= CHUNK_GENERATION_DISTANCE; y++){
                ChunkCoordinates pos = {coordinates.x + x, coordinates.y + y};
                if (loadedChunks.find(pos) == loadedChunks.end()){
                    loadedChunks[pos] = generateChunk(pos);
                    // tile variations
                    updateTileVariantsInChunk(pos, true);
                    return;
                }
            }
        }
        allChunksLoaded = true;
        doneLoading();
    }
    
    void OverworldTerrain::doneLoading(){
        if (initialLoading == true){
            initialLoading = false;
            reloadChunksAroundPoint(lastLoadChunk);

        }
    }
    
    
    void OverworldTerrain::reloadChunksAroundPoint(ChunkCoordinates coordinates){
        lastLoadChunk = coordinates;
        generateChunksAroundPoint(coordinates);
        
        allChunksLoaded = false;
        // load
        for (int i = 0; i < DISPLAYED_CHUNK_COUNT; i++){
            displayedChunks[i] = loadedChunks[{i % 3 + coordinates.x - 1,i/3 + coordinates.y - 1}];
        }

    }




    // --== Generation ==--
    
    float OverworldTerrain::getTerrainHeightValue(OverworldPosition position, const TerrainBiome& biome){
        float noiseValue = noiseMap->getNoiseValue(position, HEIGHT_NOISE_RESOLUTION);
        float spotValue = mountainNoiseMap->getNoiseValue(position, 6, 16, MOUNTAIN_NOISE_RESOLUTION);
        float roughnessValue = noiseMap->getNoiseValue(position, biome.terrainRoughnessModifier);
        return ((noiseValue * 1.5f) + (spotValue * 0.5f) + roughnessValue) / 3;
    }

    
    
    OverworldChunk* OverworldTerrain::generateChunk(ChunkCoordinates position){

        // generate some garbage data
        OverworldChunk* output = new OverworldChunk();
        output->coordinates = position;
        output->worldObjects = {};
        //output->terrainBiome = 1; // TODO
        //const TerrainBiome& biome = biomeLookup[output->terrainBiome];

        // generate tiles based on height map
        for (int x = 0; x < OVERWORLD_CHUNK_SIZE; x++){
            for (int y = 0; y < OVERWORLD_CHUNK_SIZE; y++){


                OverworldPosition tilePosition = {position.x * OVERWORLD_CHUNK_SIZE + x, position.y * OVERWORLD_CHUNK_SIZE + y};
                
                const TerrainBiome& biome = biomeLookup[biomeMap->getBiome(tilePosition)];
                
                float terrainHeightValue = getTerrainHeightValue(tilePosition, biome);
                
                // add generation
                output->tiles[x][y] = {generateTile(terrainHeightValue, biome), 0};
            }
        }

        
        // generate patterns
        for (int x = -1; x <= 1; x++){
            for (int y = -1; y <= 1; y++){
                addGenerationPatternsForChunk(output->patterns, {position.x + x, position.y + y});
            }
        }
        

        // patterns
        for (PatternGenerationObject& p : output->patterns){
            applyPattern(output->tiles, output->worldObjects, p, position);
        }
        

        

        return output;

    }

    void OverworldTerrain::addGenerationPatternsForChunk(std::vector<PatternGenerationObject>& patterns, ChunkCoordinates position){
        for (int x = 0; x < OVERWORLD_CHUNK_SIZE; x++){
            for (int y = 0; y < OVERWORLD_CHUNK_SIZE; y++){
 

                addGenerationPattern(patterns, position, x, y);

            }
        }
    }



    void OverworldTerrain::addGenerationPattern(std::vector<PatternGenerationObject>& patterns, ChunkCoordinates& chunkPosition, int x, int y){
        // prep values
        OverworldPosition position = {chunkPosition.x * OVERWORLD_CHUNK_SIZE + x, chunkPosition.y * OVERWORLD_CHUNK_SIZE + y};
        const TerrainBiome& biome = biomeLookup[biomeMap->getBiome(position)];
        float terrainHeightValue = getTerrainHeightValue(position, biome);
        float structureValue = structureNoiseMap->getNoiseValue(position, 1, 1, 16);
                
        // structures
        if (structureValue > 0.9){
            patterns.push_back({position, PATTERN_DUNGEON});
            return;
        }


        // water generation
        else if (terrainHeightValue < biome.waterGenerationValue){
            patterns.push_back({
                position,
                PATTERN_WATER,
            });
            return;
        }
        
        // trees
        float treeGenerationValue = treeNoiseMap->getNoiseValue(position, TREE_NOISE_RESOLUTION);
        treeGenerationValue = treeGenerationValue * (1.0f - std::abs( (terrainHeightValue * biome.treeToleranceFalloff) - biome.preferedTreeValue)) ;

        
        
        
        if (treeGenerationValue > biome.treeGenerationValue && Utils::getPseudoRandomFloat(Utils::hashVector(position.x, position.y)) <= biome.treeDensity){
            patterns.push_back({position, PATTERN_TREE});
            return;
        }

        
        
    }

    int OverworldTerrain::generateTile(float terrainHeightValue, const TerrainBiome& biome){
        
        if (terrainHeightValue > biome.mountainValue){
            return biome.mountainTile;
        }
        else if (terrainHeightValue > biome.stoneValue){
            return biome.stoneTile;
        }
        else if (terrainHeightValue < biome.shoreValue){
            return biome.shoreTile;
        }
        
        return biome.defaultTile;
        
    }








    // --== Pattern code ==--
    bool isInChunk(int x, int y){
        return x >= 0 && y >= 0 && x < OVERWORLD_CHUNK_SIZE && y < OVERWORLD_CHUNK_SIZE;
    }
    
    bool isTileSolid(int tileId){
        return tileLookupTable[tileId].blocksMovement;
    }
    
    void tryWatterPattern(InWorldTile tiles[OVERWORLD_CHUNK_SIZE][OVERWORLD_CHUNK_SIZE], int tileId, const TerrainBiome& biome, int x, int y){
        
        
        if (isInChunk(x, y) && (!tileLookupTable[tiles[x][y].tileId].blocksMovement || tileId == biome.waterTile)){
            tiles[x][y].tileId = tileId;
        }
    }

    
    
    void OverworldTerrain::applyPattern(InWorldTile tiles[OVERWORLD_CHUNK_SIZE][OVERWORLD_CHUNK_SIZE],  std::vector<OverworldObject>& objects, PatternGenerationObject& pattern, ChunkCoordinates& chunkPosition){
        OverworldPosition pos = pattern.position;
        pos.x -= chunkPosition.x * OVERWORLD_CHUNK_SIZE;
        pos.y -= chunkPosition.y * OVERWORLD_CHUNK_SIZE;
        
        const TerrainBiome& biome = biomeLookup[biomeMap->getBiome(pattern.position)];
        
        switch (pattern.patern) {
            case PATTERN_WATER:
                tryWatterPattern(tiles, biome.waterTile, biome, pos.x, pos.y);
                tryWatterPattern(tiles, biome.sandTile, biome, pos.x - 1, pos.y);
                tryWatterPattern(tiles, biome.sandTile, biome, pos.x + 1, pos.y);
                tryWatterPattern(tiles, biome.sandTile, biome, pos.x, pos.y - 1);
                tryWatterPattern(tiles, biome.sandTile, biome, pos.x, pos.y + 1);
                break;
            case PATTERN_TREE:
                if (isInChunk(pos.x, pos.y) && !isTileSolid(tiles[pos.x][pos.y].tileId)){objects.push_back(generateOverworldObject(biome, OBJECT_TREE, pattern.position));};
                break;
            case PATTERN_DUNGEON:
                    if (isInChunk(pos.x, pos.y) && !isTileSolid(tiles[pos.x][pos.y].tileId)){objects.push_back(generateOverworldObject(biome, OBJECT_DUNGEON, pattern.position));};
                    tryWatterPattern(tiles, biome.stoneTile, biome, pos.x, pos.y);
                    tryWatterPattern(tiles, biome.stoneTile, biome, pos.x + 1, pos.y);
                    tryWatterPattern(tiles, biome.stoneTile, biome, pos.x - 1, pos.y);
                    tryWatterPattern(tiles, biome.stoneTile, biome, pos.x, pos.y - 1);
                    tryWatterPattern(tiles, biome.stoneTile, biome, pos.x, pos.y + 1);

                break;
        }
    }













    // --== Drawing ==--
    void OverworldTerrain::draw(){
        for (int i = 0; i < DISPLAYED_CHUNK_COUNT; i++){
            drawChunk(displayedChunks[i]);
        }
    }


    void OverworldTerrain::drawTile(const InWorldTile& tile, Vector2& position){
        
        const OverworldTile tileData = tileLookupTable[tile.tileId];

        std::string sprite;

        if (tile.spriteVariant == 0){
            sprite = tileData.sprite;
        }else {
            sprite = concatSprite(tileData.sprite.c_str(), tile.spriteVariant);
        }
        

        if (tileData.animation.animates){
            sprite += "_" + std::to_string(Utils::animationTimer(tileData.animation.startFrame, tileData.animation.endFrame, tileData.animation.ticksPerFrame));
        }
        
        Drawing::get()->drawTexture(sprite, position, false, 1, 0, WHITE, DrawingLayers::LAYER_WORLD);

    }


    void OverworldTerrain::drawChunk(OverworldChunk* chunk){
        
        Drawing::get()->drawText(std::to_string(chunk->coordinates.x) + ", " + std::to_string(chunk->coordinates.y), {(float)chunk->coordinates.x * OVERWORLD_CHUNK_SIZE * OVERWORLD_TILE_SIZE, (float)chunk->coordinates.y * OVERWORLD_CHUNK_SIZE * OVERWORLD_TILE_SIZE}, 1, WHITE);
        
        // draw tiles
        for (int x = 0; x < OVERWORLD_CHUNK_SIZE; x++){
            for (int y = 0; y < OVERWORLD_CHUNK_SIZE; y++){
                const InWorldTile tile = chunk->tiles[x][y];
                Vector2 position = {((chunk->coordinates.x * OVERWORLD_CHUNK_SIZE) + x) * OVERWORLD_TILE_SIZE, ((chunk->coordinates.y * OVERWORLD_CHUNK_SIZE) + y) * OVERWORLD_TILE_SIZE};
                drawTile(tile, position);
            }
        }

        // draw objects
        for (OverworldObject object : chunk->worldObjects){
            Drawing::get()->drawTexture(object.sprite, {object.position.x * OVERWORLD_TILE_SIZE, object.position.y * OVERWORLD_TILE_SIZE}, false, 1, 0, WHITE, LAYER_OBJECT);
        }
    }


    std::vector<OverworldObject*> OverworldTerrain::getNearbyObjects(OverworldPosition position, float radius){
        int chunkX = Utils::dividePosition(position.x, OVERWORLD_CHUNK_SIZE);
        int chunkY = Utils::dividePosition(position.y, OVERWORLD_CHUNK_SIZE);

        std::vector<OverworldObject*> output;


        for (int x = -1; x <= 1; x++){
            for (int y = -1; y <= 1; y++){
                for (TerrainGeneration::OverworldObject& object : loadedChunks[{chunkX + x, chunkY + y}]->worldObjects){
                    if (Utils::pythagoras(position.x, position.y, object.position.x, object.position.y) < radius){
                        output.push_back(&object);
                    }
                }
            }
        }

        return output;
    }


    // tile variant updates
    void OverworldTerrain::updateTileVariantsInChunk(ChunkCoordinates position, bool recurse){
        if (getChunk(position) == nullptr){
            return;
        }
        
        for (int x = 0; x < OVERWORLD_CHUNK_SIZE; x++){
            for (int y = 0; y < OVERWORLD_CHUNK_SIZE; y++){
                updateTileVariant({position.x * OVERWORLD_CHUNK_SIZE + x, position.y * OVERWORLD_CHUNK_SIZE + y});
            }
        }

        if (recurse){
            updateTileVariantsInChunk({position.x - 1, position.y}, false);
            updateTileVariantsInChunk({position.x + 1, position.y}, false);
            updateTileVariantsInChunk({position.x, position.y - 1}, false);
            updateTileVariantsInChunk({position.x, position.y + 1}, false);

        }
    }

    // bad code makes you apreciate standards more
    void OverworldTerrain::updateTileVariant(OverworldPosition position){
        InWorldTile* target = getTile(position);
        // if target is null just crash        
        
        TileSurroundInfo info = getTileSurroundInfo(position);
        OverworldTile tileData = tileLookupTable[target->tileId];
        int connections = info.down + info.left + info.right + info.up;

        

        if (tileData.type == TILESET_NONE){
            target->spriteVariant = 0;
            return;
        }

        // dot tile
        if (connections == 0){
            target->spriteVariant = 9 + tileData.tileVariations;
            return;

        }
        // surounded tile
        if (connections == 4){
            target->spriteVariant = getPseudoRandomInt(1, tileData.tileVariations, hashVector(position.x, position.y)); 
            return;
        }

        // 3 connections
        if (connections == 3){

            if (!info.up){
                target->spriteVariant = 7 + tileData.tileVariations;
                return;
            }else if (!info.down){
                target->spriteVariant = 2 + tileData.tileVariations;
                return;
            }else if (!info.left){
                target->spriteVariant = 4 + tileData.tileVariations;
                return;
            }else{
                target->spriteVariant = 5 + tileData.tileVariations;
                return;
            }

        }

        // straigth
        if (connections == 2 &&((info.up && info.down) || (info.left && info.right))){
            if (tileData.type == TILESET_ADVANCED){
                if (info.up){
                    target->spriteVariant = 13 + tileData.tileVariations;
                    return;
                }else {
                    target->spriteVariant = 10 + tileData.tileVariations;
                    return;
                }
            }else {
                target->spriteVariant = 9 + tileData.tileVariations;
                return;
            }
        }

        // corners
        if (connections == 2){
            target->spriteVariant = 1 + tileData.tileVariations;

            if (info.down){
                target->spriteVariant += 5;
            }

            if (info.left){
                target->spriteVariant += 2;
            }

            return;
        }

        // dead ends
        if (connections == 1){
            if (tileData.type == TILESET_ADVANCED){
                if (info.up){
                    target->spriteVariant = 15 + tileData.tileVariations;
                    return;
                }else if (info.down) {
                    target->spriteVariant = 14 + tileData.tileVariations;
                    return;
                }else if (info.left){
                    target->spriteVariant = 12 + tileData.tileVariations;
                    return;
                }else if (info.right){
                    target->spriteVariant = 11 + tileData.tileVariations;
                    return;
                }
            }else {
                target->spriteVariant = 9 + tileData.tileVariations;
                return;
            }
        }




        target->spriteVariant = 1;

        
    }

    bool compareTiles(InWorldTile* target, InWorldTile* other){
        return target != nullptr && other != nullptr && target->tileId == other->tileId;
    }
    TileSurroundInfo OverworldTerrain::getTileSurroundInfo(OverworldPosition position){
        
        
        InWorldTile* targetTile = getTile(position);
        TileSurroundInfo output = {};

        InWorldTile* up = getTile({position.x, position.y - 1});
        InWorldTile* down = getTile({position.x, position.y + 1});
        InWorldTile* left = getTile({position.x - 1, position.y});
        InWorldTile* right = getTile({position.x + 1, position.y});


        output.up = compareTiles(targetTile, up);
        output.down = compareTiles(targetTile, down);
        output.left = compareTiles(targetTile, left);
        output.right = compareTiles(targetTile, right);

        return output;
    }
    

    OverworldChunk* OverworldTerrain::getChunk(ChunkCoordinates position){
        if (loadedChunks.find(position) == loadedChunks.end()){
            return nullptr;
        }
        return loadedChunks.at(position);
    }

    InWorldTile* OverworldTerrain::getTile(OverworldPosition position){
        int chunkX = Utils::dividePosition(position.x, OVERWORLD_CHUNK_SIZE);
        int chunkY = Utils::dividePosition(position.y, OVERWORLD_CHUNK_SIZE);

        OverworldChunk* c = getChunk({chunkX, chunkY});


        int adjustedX = position.x - (chunkX * OVERWORLD_CHUNK_SIZE);
        int adjustedY = position.y - (chunkY * OVERWORLD_CHUNK_SIZE);

        if (c != nullptr){
            return &c->tiles[adjustedX][adjustedY];
        }
        return nullptr;
    }

}