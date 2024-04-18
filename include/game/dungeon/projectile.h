#ifndef DUNGEON_PROJECTILE
#define DUNGEON_PROJECTILE

#include <string>
#include "raylib.h"
#include "engine/utils.h"

namespace DungeonCode{
    class Projectile{
        private:
            std::string sprite;
            Vector2 position;
            float velocity;
            float deceleration;
            float rotation;
            bool alliedWithPlayer;
            float damage;
            bool destroy = false;
            Utils::GenericTimer pierceTimer = Utils::GenericTimer(5);

        public:
            Projectile(std::string sprite, Vector2 position, float rotation, float velocity, float deceleration, float damage, bool alliedWithPlayer);
            void update();
            bool& shouldDestroy();
            Vector2& getPosition();
            bool& isAlliedWithPlayer();
            float& getDamage();
            float& getRotation();
            bool canDamage();
            void resetPierceTimer();
    };
}

#endif