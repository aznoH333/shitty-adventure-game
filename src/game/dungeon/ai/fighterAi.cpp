#include "game/dungeon/ai/fighterAi.h"
#include "game/dungeon/dungeon.h"
#include "game/dungeon/dungeonUtils.h"




namespace EnemyAiCode{
    using namespace DungeonCode;
    using namespace DungeonUtils;

    constexpr Vector2 DETECTION_BOX_SIZE = {DungeonEnemy::ENEMY_SIZE.x, DungeonEnemy::ENEMY_SIZE.y - 1};

    void FighterAi::update(DungeonEnemy* self){
        DungeonPlayer* p = Dungeon::get()->getPlayer();
        Dungeon* d = Dungeon::get();


        bool isNearPlayer = Utils::pythagoras(p->getPosition().x, p->getPosition().y, self->getPosition().x, self->getPosition().y) < 180;

        float moveBy = self->getMovementSpeed() * (self->getFlip() * 2 - 1);
        float detectPosition = (DungeonEnemy::ENEMY_SIZE.x * (self->getFlip() * 2 - 1));

        bool isOnGround = d->collidesWithDungeon({self->getPosition().x, self->getPosition().y + DungeonEnemy::ENEMY_SIZE.y + 1.0f}, {DungeonEnemy::ENEMY_SIZE.x, 1.0f}, true);
        

        if (d->collidesWithDungeon({self->getPosition().x + detectPosition, self->getPosition().y}, DETECTION_BOX_SIZE, false) || 
           !d->collidesWithDungeon({self->getPosition().x + detectPosition, self->getPosition().y + DungeonEnemy::ENEMY_SIZE.y}, DETECTION_BOX_SIZE, false))
        {
            if (isNearPlayer){
                if (isOnGround){
                    velocity.y -= 3.5f;
                    isOnGround = false;
                }
                
            }else {
                self->getFlip() = !self->getFlip();
            }
        }else {
            velocity.x = moveBy;
        }

        if (isNearPlayer){
            self->getFlip() = self->getPosition().x < p->getPosition().x;
        }



        if (isOnGround){
            velocity.y = 0;
        }else{
            velocity.y += 0.1;
        }


        universalDungeonCollidingUpdate(self->getPosition(), velocity, DungeonEnemy::ENEMY_SIZE, false);

        


        
    }
}