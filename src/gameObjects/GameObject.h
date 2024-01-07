#ifndef GAMEOBJECT
#define GAMEOBJECT

#include <raylib-cpp.hpp>


enum ObjectIdentifier {
    EnemyFlag,
    PlayerFlag,
    PlayerProjectileFlag,
    EnemyProjectileFlag
};



class GameObject{
    private:
        bool destructionMark = false;
    
    protected:
        Vector2 pos;
        Vector2 size;
        ObjectIdentifier identifier;

    public:
        GameObject(Vector2 pos, Vector2 size, ObjectIdentifier identifier);
        virtual void update();
        virtual void onDestroy();
        virtual void onCollide(GameObject* other);
        bool shouldDestroy();
        void destroy();
        Vector2 getPos();
        Vector2 getSize();
        ObjectIdentifier getObjectIdentifier();

};

#endif