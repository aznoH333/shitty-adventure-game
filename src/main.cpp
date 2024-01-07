#include <raylib-cpp.hpp>
#include "sprites/SpriteManager.h"
#include "gameObjects/GameObjectManager.h"

int main() {
    
    // Initialization
    const int screenWidth = 1280;
    const int screenHeight = 720;

    raylib::Window w(screenWidth, screenHeight, "aznoH's RayLib 2D game template");
    
    SetTargetFPS(60);
    while (!w.ShouldClose())
    {
        BeginDrawing();
        
        ClearBackground({237, 147, 99, 255});
        
        
        GameObjectManager::getInstance()->update();
        
        EndDrawing();
    }
    
    GameObjectManager::getInstance()->dispose();
    SpriteManager::getInstance()->dispose();

    return 0;
}