#include "game/dungeon/projectile.h"
#include <cmath>
#include "engine/drawing.h"

namespace DungeonCode {
    Projectile::Projectile(std::string sprite, Vector2 position, float rotation, float velocity, float deceleration, float damage, bool alliedWithPlayer){
        this->sprite = sprite;
        this->position = position;
        this->rotation = rotation;
        this->velocity = velocity;
        this->deceleration = deceleration;
        this->damage = damage;
        this->alliedWithPlayer = alliedWithPlayer;
        this->destroy = false;
    }


    void Projectile::update(){
        // move
        this->position.x += std::cos(rotation) * this->velocity;
        this->position.y += std::sin(rotation) * this->velocity;
        this->velocity /= 1 + this->deceleration;

        // state update
        if (this->velocity <= 0.1f){
            this->destroy = true;
        }
        pierceTimer.progress();

        // draw
        Drawing::get()->drawTexture(this->sprite, this->position, 0, 1.0f, this->rotation * RAD2DEG, WHITE, LAYER_PROJECTILES);
    }

    bool& Projectile::shouldDestroy(){
        return destroy;
    }

    Vector2& Projectile::getPosition(){
        return this->position;
    }

    bool& Projectile::isAlliedWithPlayer(){
        return this->alliedWithPlayer;
    }

    float& Projectile::getDamage(){
        return this->damage;
    }

    float& Projectile::getRotation(){
        return this->rotation;
    }

    bool Projectile::canDamage(){
        return pierceTimer.isReady();
    }

    void Projectile::resetPierceTimer(){
        pierceTimer.reset();
    }
}