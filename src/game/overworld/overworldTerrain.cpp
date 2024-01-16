#include "game/overworld/overworldTerrain.h"
#include <iostream>

namespace TerrainGeneration {


    OverworldTerrain::OverworldTerrain(){
        
        // dumbass tried to generate the chunks before the noise
        noiseMap = new ValueNoiseMap(1020);
        mountainNoiseMap = new SpotNoiseMap(960);
        treeNoiseMap = new ValueNoiseMap(3547);
        structureNoiseMap = new SpotNoiseMap(1984);
        worldLoadingThread = std::thread(&OverworldTerrain::worldLoadingFunction, this);
        // init some garbage data
        for (int i = 0; i < DISPLAYED_CHUNK_COUNT; i++){
            displayedChunks[i] = generateChunk({i % 3,i/3});
        }

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
    }



    // --== Update ==--
    void OverworldTerrain::update(){
        draw();
        if (Utils::gameTickCounter % DEFAULT_WORLD_LOADING_INTERVAL == 0){
            shouldAttemptLoading = true;
        }
    }

    

    // --== chunk loading ==--
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
                    return;
                }
            }
        }
        allChunksLoaded = true;
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
    OverworldChunk* OverworldTerrain::generateChunk(ChunkCoordinates position){
        std::cout << "Started generating chunk... " << position.x << ", " << position.y << "\n";
        
        // generate some garbage data
        OverworldChunk* output = new OverworldChunk();
        output->coordinates = position;
        output->worldObjects = {};
        output->terrainBiome = 0; // TODO
        const TerrainBiome& biome = biomeLookup[output->terrainBiome];

        // generate tiles based on height map
        for (int x = 0; x < OVERWORLD_CHUNK_SIZE; x++){
            for (int y = 0; y < OVERWORLD_CHUNK_SIZE; y++){
                
                OverworldPosition tilePosition = {position.x * OVERWORLD_CHUNK_SIZE + x, position.y * OVERWORLD_CHUNK_SIZE + y};
                
                float noiseValue = noiseMap->getNoiseValue(tilePosition, HEIGHT_NOISE_RESOLUTION);
                float spotValue = mountainNoiseMap->getNoiseValue(tilePosition, 6, 16, MOUNTAIN_NOISE_RESOLUTION);
                float terrainHeightValue = noiseValue;
                
                
                // add generation
                output->tiles[x][y] = generateTile(terrainHeightValue, biome);
            }
        }

        /*
        // generate patterns
        for (int x = -1; x <= 1; x++){
            for (int y = -1; y <= 1; y++){
                addGenerationPatternsForChunk(output->patterns, {position.x + x, position.y + y}, biome);
            }
        }
        

        // patterns
        for (PatternGenerationObject& p : output->patterns){
            applyPattern(output->tiles, output->worldObjects, p, position, biome);
        }
        */


        return output;

    }

    void OverworldTerrain::addGenerationPatternsForChunk(std::vector<PatternGenerationObject>& patterns, ChunkCoordinates position, const TerrainBiome& biome){
        for (int x = 0; x < OVERWORLD_CHUNK_SIZE; x++){
            for (int y = 0; y < OVERWORLD_CHUNK_SIZE; y++){
                OverworldPosition tilePosition = {position.x * OVERWORLD_CHUNK_SIZE + x, position.y * OVERWORLD_CHUNK_SIZE + y};
                
                float noiseValue = noiseMap->getNoiseValue(tilePosition, HEIGHT_NOISE_RESOLUTION);
                float spotValue = mountainNoiseMap->getNoiseValue(tilePosition, 6, 16, 16);
                float terrainHeightValue = std::max(noiseValue, spotValue);


                float treeGenerationValue = treeNoiseMap->getNoiseValue(tilePosition, TREE_NOISE_RESOLUTION) - structureNoiseMap->getNoiseValue(tilePosition, 2, 3, 16);
                float structureValue = structureNoiseMap->getNoiseValue(tilePosition, 1, 1, 16);
                
                addGenerationPattern(patterns, terrainHeightValue, treeGenerationValue, structureValue, tilePosition, biome);

            }
        }
    }



    void OverworldTerrain::addGenerationPattern(std::vector<PatternGenerationObject>& patterns, float terrainHeightValue, float treeGenerationValue, float structureGenerationValue, OverworldPosition position, const TerrainBiome& biome){
        
        // structures
        if (structureGenerationValue > 0.9){
            patterns.push_back({position, PATTERN_DUNGEON});
            return;
        }
        
        // trees
        else if (treeGenerationValue > biome.treeGenerationValue){
            patterns.push_back({position, PATTERN_TREE});
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
    
    void tryWatterPattern(int tiles[OVERWORLD_CHUNK_SIZE][OVERWORLD_CHUNK_SIZE], int tileId, const TerrainBiome& biome, int x, int y){
        
        
        if (isInChunk(x, y) && (tiles[x][y] == biome.defaultTile || tileId == biome.waterTile)){
            tiles[x][y] = tileId;
        }
    }

    
    
    void OverworldTerrain::applyPattern(int tiles[OVERWORLD_CHUNK_SIZE][OVERWORLD_CHUNK_SIZE],  std::vector<OverworldObject>& objects, PatternGenerationObject& pattern, ChunkCoordinates& chunkPosition, const TerrainBiome& biome){
        OverworldPosition pos = pattern.position;
        pos.x -= chunkPosition.x * OVERWORLD_CHUNK_SIZE;
        pos.y -= chunkPosition.y * OVERWORLD_CHUNK_SIZE;
        
        
        switch (pattern.patern) {
            case PATTERN_WATER:
                tryWatterPattern(tiles, biome.waterTile, biome, pos.x, pos.y);
                tryWatterPattern(tiles, biome.sandTile, biome, pos.x - 1, pos.y);
                tryWatterPattern(tiles, biome.sandTile, biome, pos.x + 1, pos.y);
                tryWatterPattern(tiles, biome.sandTile, biome, pos.x, pos.y - 1);
                tryWatterPattern(tiles, biome.sandTile, biome, pos.x, pos.y + 1);
                break;
            case PATTERN_TREE:
                if (isInChunk(pos.x, pos.y) && !isTileSolid(tiles[pos.x][pos.y])){objects.push_back(generateOverworldObject(biome, OBJECT_TREE, pattern.position));};
                break;
            case PATTERN_DUNGEON:
                    if (isInChunk(pos.x, pos.y) && !isTileSolid(tiles[pos.x][pos.y])){objects.push_back(generateOverworldObject(biome, OBJECT_DUNGEON, pattern.position));};
                    tryWatterPattern(tiles, biome.sandTile, biome, pos.x, pos.y);
                    tryWatterPattern(tiles, biome.sandTile, biome, pos.x + 1, pos.y);
                    tryWatterPattern(tiles, biome.sandTile, biome, pos.x - 1, pos.y);
                    tryWatterPattern(tiles, biome.sandTile, biome, pos.x, pos.y - 1);
                    tryWatterPattern(tiles, biome.sandTile, biome, pos.x, pos.y + 1);

                break;
        }
    }













    // --== Drawing ==--
    void OverworldTerrain::draw(){
        for (int i = 0; i < DISPLAYED_CHUNK_COUNT; i++){
            drawChunk(displayedChunks[i]);
        }
    }

    void OverworldTerrain::drawChunk(OverworldChunk* chunk){
        // draw tiles
        for (int x = 0; x < OVERWORLD_CHUNK_SIZE; x++){
            for (int y = 0; y < OVERWORLD_CHUNK_SIZE; y++){
                const OverworldTile tile = tileLookupTable[chunk->tiles[x][y]];
                Vector2 position = {((chunk->coordinates.x * OVERWORLD_CHUNK_SIZE) + x) * OVERWORLD_TILE_SIZE, ((chunk->coordinates.y * OVERWORLD_CHUNK_SIZE) + y) * OVERWORLD_TILE_SIZE};
                Drawing::get()->drawTexture(tile.sprite, position, false, 1, 0, WHITE, DrawingLayers::LAYER_WORLD);
            
            }
        }

        // draw objects
        for (OverworldObject object : chunk->worldObjects){
            Drawing::get()->drawTexture(object.sprite, {object.position.x * OVERWORLD_TILE_SIZE, object.position.y * OVERWORLD_TILE_SIZE}, false, 1, 0, WHITE, LAYER_OBJECT);
        }
    }

}