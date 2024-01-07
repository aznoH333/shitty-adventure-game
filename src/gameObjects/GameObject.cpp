#include "GameObject.h"

GameObject::GameObject(Vector2 pos, Vector2 size, ObjectIdentifier identifier){
    this->pos = pos;
    this->size = size;
    this->identifier = identifier;
}


// virtual functions
void GameObject::update(){}
void GameObject::onDestroy(){}
void GameObject::onCollide(GameObject* other){}


bool GameObject::shouldDestroy(){
    return destructionMark;
}

void GameObject::destroy(){
    destructionMark = true;
}

Vector2 GameObject::getPos(){
    return pos;
}

Vector2 GameObject::getSize(){
    return size;
}

ObjectIdentifier GameObject::getObjectIdentifier(){
    return identifier;
}