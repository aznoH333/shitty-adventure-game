#ifndef GAMEOBJECT_MANAGER
#define GAMEOBJECT_MANAGER

#include "GameObject.h"
#include <list>

class GameObjectManager{
    private:
        static GameObjectManager* instance;
        std::list<GameObject*> objects;


    public:
        static GameObjectManager* getInstance();
        void update();
        void addGameObject(GameObject* object);
        void dispose();
};

#endif