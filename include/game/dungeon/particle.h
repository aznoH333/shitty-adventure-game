#ifndef PARTICLE
#define PARTICLE

#include "raylib.h"
#include <string>

namespace DungeonCode {
    class ParticleBuilder{
        public:
            Vector2 position = {0,0};
            Vector2 velocity = {0,0};
            std::string sprite = "";
            int lifeTime = 0;
            int startingLifetime = 0;
            bool decays = true;

            // color
            bool isColorAnimated = false;
            Color startColor = WHITE;
            Color endColor = WHITE; 

            // width
            Vector2 width = {15.0f, 15.0f};

            // animation
            bool isAnimated = false;
            int animationStart = 0;
            int animationEnd = 0;
            
            // gravity
            Vector2 gravity = {0,0};

            // rotation
            float rotationSpeed = 0.0f;
            float rotation = 0.0f;


            ParticleBuilder(Vector2 position, std::string sprite, int lifeTime);
            void setAnimation(int animationStart, int animationEnd);
            void setColor(Color startColor, Color endColor);
            void setVelocity(Vector2 velocity);
            void setWidth(Vector2 width);
            void setGravity(Vector2 gravity);
            void setRotation(float rotation, float rotationSpeed);
            void disableDecay();
    };
    
    
    
    class Particle{
        private:
            ParticleBuilder* data;
            bool destroy = false;


        public:
            Particle(ParticleBuilder* builder);
            void update();
            void dispose();
            bool& shouldBeDestroyed();
            Vector2& getPosition();
    };
}

#endif