#include "game/overworldTerrain.h"


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

        std::vector<PatternGenerationObject> patterns = {};

        for (int x = 0; x < OVERWORLD_CHUNK_SIZE; x++){
            for (int y = 0; y < OVERWORLD_CHUNK_SIZE; y++){
                output->tiles[x][y] = generateTile({position.x * OVERWORLD_CHUNK_SIZE + x, position.y * OVERWORLD_CHUNK_SIZE + y}, {x, y}, patterns);
            }
        }
        // patterns
        for (PatternGenerationObject& p : patterns){
            applyPattern(output->tiles, p);
        }


        // generate some garbage objects
        for (int i = 0; i < 3; i++){
            output->worldObjects.push_back(OverworldObject{"placeholders_6", {position.x * OVERWORLD_CHUNK_SIZE + GetRandomValue(0, OVERWORLD_CHUNK_SIZE), position.y * OVERWORLD_CHUNK_SIZE + GetRandomValue(0, OVERWORLD_CHUNK_SIZE)}});
        }


        return output;

    }



    int OverworldTerrain::generateTile(OverworldPosition position, OverworldPosition inChunkPosition, std::vector<PatternGenerationObject>& patterns){
        
        float noiseValue = noiseMap->getNoiseValue(position, terrainNoiseResolution);
        float spotValue = mountainNoiseMap->getNoiseValue(position, 6, 16);
        float terrainHeightValue = std::max(noiseValue, spotValue);
        
        // mountain generation
        if (terrainHeightValue > 0.7){
            return 2;
        }
        /* else if (terrainHeightValue > 0.6){
            return stone value
        }*/



        // water generation
        if (terrainHeightValue < 0.2){
            patterns.push_back({
                inChunkPosition,
                PATTERN_WATER,
            });
        }

        return 0;
    }

    void tryWatterPattern(int tiles[OVERWORLD_CHUNK_SIZE][OVERWORLD_CHUNK_SIZE], int tileId, int x, int y){
        
        
        if (x >= 0 && y >= 0 && x < OVERWORLD_CHUNK_SIZE && y < OVERWORLD_CHUNK_SIZE && (tiles[x][y] == 0 || tileId == 3)){
            tiles[x][y] = tileId;

        }
    }

    void OverworldTerrain::applyPattern(int tiles[OVERWORLD_CHUNK_SIZE][OVERWORLD_CHUNK_SIZE], PatternGenerationObject& pattern){
        OverworldPosition& pos = pattern.position;
        switch (pattern.patern) {
            case PATTERN_WATER:
                tryWatterPattern(tiles, 3, pos.x, pos.y);
                tryWatterPattern(tiles, 1, pos.x - 1, pos.y);
                tryWatterPattern(tiles, 1, pos.x + 1, pos.y);
                tryWatterPattern(tiles, 1, pos.x, pos.y - 1);
                tryWatterPattern(tiles, 1, pos.x, pos.y + 1);


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