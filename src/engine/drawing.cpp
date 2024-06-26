
#include "engine/drawing.h"
#include <filesystem>
#include <queue>
#include <raylib-cpp.hpp>
#include <vector>
#include "engine/utils.h"
#include <iostream>

Drawing* Drawing::instance = 0;

// --== boiler plate ==--
void Drawing::init(std::string assetsFolder, int renderLayerCount, int screenWidth, int screenHeight){
    Drawing::instance = new Drawing();


    Drawing::instance->targetTexture = LoadRenderTexture(screenWidth, screenHeight);


    Drawing::instance->loadAllTexturesFromDirectory(std::filesystem::path{assetsFolder + "sprites/"});

    Drawing::instance->camera = Camera2D{{((float)screenWidth) / 2.0f - 32.0f, ((float) screenHeight) / 2.0f - 32.0f}, {0,0}, 0, DEFAULT_CAMERA_ZOOM};
    Drawing::instance->staticCamera = Camera2D{{0.0f,0.0f}, {0,0}, 0, DEFAULT_CAMERA_ZOOM};
    
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
void renderTexture(RenderData& data, float screenShakeOffset){
    
    Vector2 calculatedWidth = {
        abs(data.texture->width) * data.scale, abs(data.texture->height) * data.scale
    };

    Vector2 origin = {0,0};
        
    Vector2 pos = data.pos;

    bool flipHorizontaly = (data.flipSprite == 1 || data.flipSprite == 3);
    bool flipVerticaly = (data.flipSprite == 2 || data.flipSprite == 3);

    Rectangle source = {
        flipHorizontaly ? (float)data.texture->width : 0,
        flipVerticaly ? (float)data.texture->height : 0, 
        flipHorizontaly ? -(float)data.texture->width : (float)data.texture->width, 
        flipVerticaly ? -(float)data.texture->height : (float)data.texture->height
    };
        
        
    DrawTexturePro(*data.texture, 
        source, 
        {pos.x + screenShakeOffset, pos.y , calculatedWidth.x, calculatedWidth.y},
        {0.0f,0.0f + (data.texture->height * flipVerticaly)}, data.rotation, data.color);
}

void Drawing::renderText(TextRenderData& text){
    DrawText(text.text.c_str(), (int)text.position.x, (int)text.position.y, text.scale, text.color);
}


void renderLayer(std::queue<RenderData>& layer, float screenShakeOffset){
    while (!layer.empty()){
        renderTexture(layer.front(), screenShakeOffset);
        layer.pop();
    }
}


void Drawing::render(){

    // screen shake
    float screenShakeOffset = 0.0;
    screenShake -= screenShake > 0;
    screenShakeOffset = std::sin(screenShake) * screenShake;


     // draw to texture
    BeginTextureMode(targetTexture);
        BeginMode2D(camera);
        ClearBackground(backgroundColor);
        
        for (int i = 0; i < renderLayerCount; i++){
            renderLayer(renderQueue[i], screenShakeOffset);
        }

        while (!textQueue.empty()){
            renderText(textQueue.front());
            textQueue.pop();
        }

        EndMode2D();
        // render statics
        BeginMode2D(staticCamera);
            renderLayer(staticQueue, 0.0f);
        EndMode2D();

    EndTextureMode();

    // draw texture
    BeginDrawing();
        DrawTexturePro(targetTexture.texture, (Rectangle){ 0.0f, 0.0f, (float)targetTexture.texture.width, (float)-targetTexture.texture.height },
                           (Rectangle){ (GetScreenWidth() - ((float)gameScreenWidth*screenScale))*0.5f, (GetScreenHeight() - ((float)gameScreenHeight*screenScale))*0.5f,
                           (float)gameScreenWidth*screenScale, (float)gameScreenHeight*screenScale }, (Vector2){ 0, 0 }, 0.0f, WHITE);
    EndDrawing();


}

// --== Drawing functions
void Drawing::drawTexture(std::string sprite, Vector2 pos, int flipSprite, float scale, float rotation, Color color, int layer){
    if (textures.find(sprite) == textures.end()){
        std::cout << sprite << " NOT FOUND\n";
    }
    renderQueue[layer].push(RenderData{&textures.at(sprite), pos, scale, color, rotation, flipSprite});
}


void Drawing::drawText(std::string text, Vector2 pos, float scale, Color color){
    textQueue.push({text, pos, (int)(DEFAULT_FONT_SIZE * scale), color});
}

void Drawing::drawTextureStatic(std::string sprite, Vector2 pos, bool flipSprite, float scale, float rotation, Color color){
    staticQueue.push(RenderData{&textures.at(sprite), pos, scale, color, rotation, flipSprite});
}


// --== Misc functions ==--
Camera2D& Drawing::getCamera(){
    return camera;
}

Vector2 Drawing::getInworldMousePosition(){
    return GetScreenToWorld2D(GetMousePosition(), camera);
}

void Drawing::addScreenShaker(int ammount){
    screenShake = std::min(screenShake + ammount, SCREEN_SHAKE_MAX);
}