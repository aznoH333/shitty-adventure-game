#include "game/dungeon/dungeonUtils.h"

using namespace DungeonCode;

namespace DungeonUtils {
    // --== universal entity functions ==--
    void universalDungeonCollidingUpdate(Vector2& position, Vector2& velocity, const Vector2& size, bool checkPlatforms){
        
        if (!Dungeon::get()->collidesWithDungeon({position.x + velocity.x, position.y}, size, false)){
            position.x += velocity.x;
        }else {
            velocity.x = 0.0f;
        }

        if (!Dungeon::get()->collidesWithDungeon({position.x, position.y + velocity.y}, size, checkPlatforms)){
            position.y += velocity.y;
        }else {
            velocity.y = 0.0f;
        }
    }


    void checkIfOnGround(Vector2& position, Vector2& velocity, bool& isOnGround){

    }

}