#include "GameObjectManager.h"


GameObjectManager* GameObjectManager::getInstance(){
    if (!instance) instance = new GameObjectManager();
    return instance;
}


void GameObjectManager::update(){
    
    // update and destroy
    objects.remove_if([](GameObject* g){
        g->update();
        if (g->shouldDestroy()){
            g->onDestroy();
            return true;
        }
        return false;
    });

    // check for collisions
    for (GameObject* g : objects){
        for (GameObject* other : objects){
            Vector2 pos = g->getPos();
            Vector2 size = g->getSize();
            Vector2 otherPos = other->getPos();
            Vector2 otherSize = other->getSize();
            
            if (g != other &&
            pos.x + size.x > otherPos.x &&
            pos.x < otherPos.x + otherSize.x &&
            pos.y + size.y > otherPos.y &&
            pos.y < otherPos.y + otherSize.y)
                g->onCollide(other);
        }
    }

}

void GameObjectManager::dispose(){
    for (GameObject* g : objects){
        delete g;
    }
    delete instance;
}


void GameObjectManager::addGameObject(GameObject* object){
    objects.push_back(object);
}

GameObjectManager* GameObjectManager::instance = 0;
