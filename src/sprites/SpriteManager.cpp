#include "SpriteManager.h"

SpriteManager::SpriteManager(){
    //loadTexture("resources/sprites/enemy.png");
}

SpriteManager* SpriteManager::getInstance(){
    if (!instance) instance = new SpriteManager();
    return instance;
}

Texture2D SpriteManager::getTexture(std::string name){
    return sprites[name];
}

void SpriteManager::loadTexture(std::string path){
    std::string name = path.substr(path.find_last_of("/")+1);
    name = name.substr(0, name.find_last_of("."));
    sprites[name] = LoadTexture(path.c_str());
}


void SpriteManager::dispose(){
    for (const auto& t : sprites){
        UnloadTexture(t.second);
    }
    delete instance;
}

void SpriteManager::drawTexture(std::string sprite, Vector2 pos, float scale, raylib::Color color){
    DrawTextureEx(getTexture(sprite), pos, 0, scale, color);
}

SpriteManager* SpriteManager::instance = 0;
