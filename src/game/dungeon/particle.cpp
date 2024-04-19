#include "game/dungeon/particle.h"
#include "engine/drawing.h"
#include "engine/utils.h"

using namespace Utils;
namespace DungeonCode {
    // --== builder ==--
    ParticleBuilder::ParticleBuilder(Vector2 position, std::string sprite, int lifeTime){
        this->position = position;
        this->sprite = sprite;
        this->lifeTime = lifeTime;
        this->startingLifetime = lifeTime;
    }

    void ParticleBuilder::setAnimation(int animationStart, int animationEnd){
        this->isAnimated = true;
        this->animationStart = animationStart;
        this->animationEnd = animationEnd;
    }

    void ParticleBuilder::setColor(Color startColor, Color endColor){
        this->startColor = startColor;
        this->endColor = endColor;
        this->isColorAnimated = true;
    }

    void ParticleBuilder::setVelocity(Vector2 velocity){
        this->velocity = velocity;
    }

    void ParticleBuilder::setWidth(Vector2 width){
        this->width = width;
    }

    void ParticleBuilder::setGravity(Vector2 gravity){
        this->gravity = gravity;
    }
    void ParticleBuilder::setRotation(float rotation, float rotationSpeed){
        this->rotation = rotation;
        this->rotationSpeed = rotationSpeed;
    }

    void ParticleBuilder::disableDecay(){
        decays = false;
    }


    // --== particle ==--
    Particle::Particle(ParticleBuilder* builder){
        data = builder;
    }

    void Particle::update(){
        // update values
        data->lifeTime -= data->decays;

        if (data->lifeTime < 0){
            destroy = true;
            return;
        }

        // position
        data->position.x += data->velocity.x;
        data->position.y += data->velocity.y;

        data->velocity.x += data->gravity.x;
        data->velocity.y += data->gravity.y;

        // rotation
        data->rotation += data->rotationSpeed;        
        
        // draw
        // animation
        float durationPercentage = ((float)data->lifeTime / data->startingLifetime);
        
        std::string sprite = data->sprite;
        if (data->isAnimated){
            sprite = concatSprite(sprite.c_str(), (int)interpolate(data->animationStart, data->animationEnd, 1 - durationPercentage));
        }
        // color
        Color color;
        if (data->isColorAnimated){
            color = {
                (unsigned char)interpolate(data->startColor.r, data->endColor.r, durationPercentage),
                (unsigned char)interpolate(data->startColor.g, data->endColor.g, durationPercentage),
                (unsigned char)interpolate(data->startColor.b, data->endColor.b, durationPercentage),
                255
            };
        }else{
            color = data->startColor;
        }
        // size
        float size = durationPercentage;
        Vector2 position = data->position;
        position.x += ((data->width.x / 2) * (1.0f - size));
        position.y += ((data->width.x / 2) * (1.0f - size));

        Drawing::get()->drawTexture(sprite, position, false, size, data->rotation, color, LAYER_EFFECTS);
    }

    void Particle::dispose(){
        delete data;
    }

    bool& Particle::shouldBeDestroyed(){
        return destroy;
    }

    Vector2& Particle::getPosition(){
        return data->position;
    }
}