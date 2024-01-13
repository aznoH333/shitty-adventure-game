#include "game/overworld/overworldTerrain.h"


namespace TerrainGeneration {


    OverworldTerrain::OverworldTerrain(){
        
        // dumbass tried to generate the chunks before the noise
        noiseMap = new ValueNoiseMap(1020);
        mountainNoiseMap = new SpotNoiseMap(960);
        
        // init some garbage data
        for (int i = 0; i < LOADED_CHUNK_COUNT; i++){
            loadedChunks[i] = generateChunk({i % 3,i/3});
        }

    }

    OverworldTerrain::~OverworldTerrain(){
        // free loaded chunks
        for (int i = 0; i < LOADED_CHUNK_COUNT; i++){
            delete loadedChunks[i];
        }

        delete noiseMap;
        delete mountainNoiseMap;
    }



    // --== Update ==--
    void OverworldTerrain::update(){

        draw();
    }






    // --== Generation ==--




    OverworldChunk* OverworldTerrain::generateChunk(ChunkCoordinates position){
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
                
                float noiseValue = noiseMap->getNoiseValue(tilePosition, terrainNoiseResolution);
                float spotValue = mountainNoiseMap->getNoiseValue(tilePosition, 6, 16, 16);
                float terrainHeightValue = std::max(noiseValue, spotValue);
                
                
                // add generation
                output->tiles[x][y] = generateTile(terrainHeightValue, biome);
            }
        }


        // generate patterns
        for (int x = -1; x <= 1; x++){
            for (int y = -1; y <= 1; y++){
                addGenerationPatternsForChunk(output->patterns, {position.x + x, position.y + y}, biome);
            }
        }
        

        // patterns
        for (PatternGenerationObject& p : output->patterns){
            applyPattern(output->tiles, p, position, biome);
        }


        // generate some garbage objects
        for (int i = 0; i < 3; i++){
            output->worldObjects.push_back(OverworldObject{"placeholders_6", {position.x * OVERWORLD_CHUNK_SIZE + GetRandomValue(0, OVERWORLD_CHUNK_SIZE), position.y * OVERWORLD_CHUNK_SIZE + GetRandomValue(0, OVERWORLD_CHUNK_SIZE)}});
        }


        return output;

    }

    void OverworldTerrain::addGenerationPatternsForChunk(std::vector<PatternGenerationObject>& patterns, ChunkCoordinates position, const TerrainBiome& biome){
        for (int x = 0; x < OVERWORLD_CHUNK_SIZE; x++){
            for (int y = 0; y < OVERWORLD_CHUNK_SIZE; y++){
                OverworldPosition tilePosition = {position.x * OVERWORLD_CHUNK_SIZE + x, position.y * OVERWORLD_CHUNK_SIZE + y};
                
                float noiseValue = noiseMap->getNoiseValue(tilePosition, terrainNoiseResolution);
                float spotValue = mountainNoiseMap->getNoiseValue(tilePosition, 6, 16, 16);
                float terrainHeightValue = std::max(noiseValue, spotValue);
                
                addGenerationPattern(patterns, terrainHeightValue, tilePosition, biome);

            }
        }
    }



    void OverworldTerrain::addGenerationPattern(std::vector<PatternGenerationObject>& patterns, float terrainHeightValue, OverworldPosition position, const TerrainBiome& biome){
        
        

        // water generation
        if (terrainHeightValue < biome.waterGenerationValue){
            patterns.push_back({
                position,
                PATTERN_WATER,
            });
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

    void tryWatterPattern(int tiles[OVERWORLD_CHUNK_SIZE][OVERWORLD_CHUNK_SIZE], int tileId, const TerrainBiome& biome, int x, int y){
        
        
        if (x >= 0 && y >= 0 && x < OVERWORLD_CHUNK_SIZE && y < OVERWORLD_CHUNK_SIZE && (tiles[x][y] == biome.defaultTile || tileId == biome.waterTile)){
            tiles[x][y] = tileId;
        }
    }

    void OverworldTerrain::applyPattern(int tiles[OVERWORLD_CHUNK_SIZE][OVERWORLD_CHUNK_SIZE], PatternGenerationObject& pattern, ChunkCoordinates& chunkPosition, const TerrainBiome& biome){
        OverworldPosition& pos = pattern.position;
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
        }
    }


    // --== Drawing ==--
    void OverworldTerrain::draw(){
        for (int i = 0; i < LOADED_CHUNK_COUNT; i++){
            drawChunk(loadedChunks[i]);
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