#ifndef DUNGEON_UTILS
#define DUNGEON_UTILS


#include "raylib.h"
#include "dungeon.h"

namespace DungeonUtils {
    // entity related functions
    void universalDungeonCollidingUpdate(Vector2& position, Vector2& velocity, const Vector2& size, bool checkPlatforms);
    void checkIfOnGround(Vector2& position, Vector2& velocity, bool& isOnGround);
}

#endif