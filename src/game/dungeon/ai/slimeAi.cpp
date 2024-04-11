#include "game/dungeon/ai/slimeAi.h"
#include "game/dungeon/dungeon.h"
#include "game/dungeon/dungeonUtils.h"

using namespace DungeonCode;
using namespace Utils;
using namespace DungeonUtils;
namespace EnemyAiCode {
    void SlimeAi::update(DungeonEnemy* self){
        DungeonPlayer* p = Dungeon::get()->getPlayer();
        Dungeon* d = Dungeon::get();


        bool isNearPlayer = Utils::pythagoras(p->getPosition().x, p->getPosition().y, self->getPosition().x, self->getPosition().y) < 180;

        float detectPosition = (DungeonEnemy::ENEMY_SIZE.x * (self->getFlip() * 2 - 1));

        bool isOnGround = d->collidesWithDungeon({self->getPosition().x, self->getPosition().y + DungeonEnemy::ENEMY_SIZE.y + 1.0f}, {DungeonEnemy::ENEMY_SIZE.x, 1.0f}, false);


        if (isNearPlayer && isOnGround){
            self->getFlip() = self->getPosition().x < p->getPosition().x;
            self->tryShooting();
        }

        if (isOnGround){
            velocity.y = 0;
            velocity.x = 0;
        }else{
            velocity.y += 0.1;
            velocity.x = boolToSign(self->getFlip()) * self->getMovementSpeed();
        }
        if (isNearPlayer){
            if (jumpCooldown > 0){
                jumpCooldown--;
            }else if (self->isAttacking() == false){
                velocity.y -= 3.0f * getRandomFloat() + 2.0f;
                jumpCooldown = std::max(120 - (self->getMovementSpeed() * 30), 20.0f);
            }
        }
        

        universalDungeonCollidingUpdate(self->getPosition(), velocity, DungeonEnemy::ENEMY_SIZE, false);

        // animatie
        if (self->isAttacking()){
            self->getSpr() = 5;
        }
        else if (isOnGround){
            self->getSpr() = Utils::animationTimer(1, 3, 10 / self->getMovementSpeed());
        }else {
            self->getSpr() = 4;
        }

    }
}