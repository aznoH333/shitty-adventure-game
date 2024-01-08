
#include "engine/drawing.h"
#include <filesystem>
#include <queue>
#include <raylib-cpp.hpp>
#include <vector>
#include "engine/utils.h"

Drawing* Drawing::instance = 0;

// --== boiler plate ==--
void Drawing::init(std::string assetsFolder, int renderLayerCount, int screenWidth, int screenHeight){
    Drawing::instance = new Drawing();


    Drawing::instance->targetTexture = LoadRenderTexture(screenWidth, screenHeight);


    Drawing::instance->loadAllTexturesFromDirectory(std::filesystem::path{assetsFolder + "sprites/"});

    Drawing::instance->camera = Camera2D{{((float)screenWidth) / 2.0f - 32.0f, ((float) screenHeight) / 2.0f - 32.0f}, {0,0}, 0, DEFAULT_CAMERA_ZOOM};
    Drawing::instance->renderLayerCount = renderLayerCount;
    


    for (int i = 0; i < renderLayerCount; i++){
        Drawing::instance->renderQueue.emplace(i, std::queue<RenderData>());
    }
}

Drawing* Drawing::get(){
    return Drawing::instance;
}

void Drawing::dispose(){
    // unload textures
    for (const auto& t : textures){
        UnloadTexture(t.second);
    }
    // unload target texture
    UnloadRenderTexture(targetTexture);

    delete instance;
}




// --== texture loading ==-
void Drawing::loadTexture(std::string path){
    std::string name = path.substr(path.find_last_of("/")+1);
    name = name.substr(0, name.find_last_of("."));

    textures.emplace(name, LoadTexture(path.c_str()));
}



void Drawing::loadAllTexturesFromDirectory(std::string spritesPath){
    std::vector<std::string> texturePaths = Utils::getFilesInFolder(spritesPath);

    for (auto a : texturePaths){
        
        loadTexture(a);
    }
    

}








// --== rendering ==--
void renderTexture(RenderData& data){
    Vector2 calculatedWidth = {
        abs(data.texture->width) * data.scale, abs(data.texture->height) * data.scale
    };

    Vector2 origin = {0,0};
        
    Vector2 pos = data.pos;
        
        
    DrawTexturePro(*data.texture, 
        {(float) data.texture->width * data.flipSprite, 0,
        (float) data.texture->width * (!data.flipSprite * 2 - 1), (float) data.texture->height}, 
        {pos.x, pos.y , calculatedWidth.x, calculatedWidth.y},
        {0,0}, data.rotation, data.color);
}

void renderLayer(std::queue<RenderData>& layer){
    while (!layer.empty()){
        renderTexture(layer.front());
        layer.pop();
    }
}



void Drawing::drawTexture(std::string sprite, Vector2 pos, bool flipSprite, float scale, float rotation, Color color, int layer){
    renderQueue[layer].push(RenderData{&textures.at(sprite), pos, scale, color, rotation, flipSprite});
}

void Drawing::render(){

     // draw to texture
    BeginTextureMode(targetTexture);
        BeginMode2D(camera);
        ClearBackground(BLACK);
        
        for (int i = 0; i < renderLayerCount; i++){
            renderLayer(renderQueue[i]);
        }

        EndMode2D();
    EndTextureMode();

    // draw texture
    BeginDrawing();
        DrawTexturePro(targetTexture.texture, (Rectangle){ 0.0f, 0.0f, (float)targetTexture.texture.width, (float)-targetTexture.texture.height },
                           (Rectangle){ (GetScreenWidth() - ((float)gameScreenWidth*screenScale))*0.5f, (GetScreenHeight() - ((float)gameScreenHeight*screenScale))*0.5f,
                           (float)gameScreenWidth*screenScale, (float)gameScreenHeight*screenScale }, (Vector2){ 0, 0 }, 0.0f, WHITE);
    EndDrawing();


}




// --== Misc functions ==--
Camera2D& Drawing::getCamera(){
    return camera;
}