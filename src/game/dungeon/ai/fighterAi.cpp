#include "game/dungeon/ai/fighterAi.h"
#include "game/dungeon/dungeon.h"


namespace EnemyAiCode{
    using namespace DungeonCode;

    void FighterAi::update(DungeonEnemy* self){
        DungeonPlayer* p = Dungeon::get()->getPlayer();
        Dungeon* d = Dungeon::get();


        //bool isNearPlayer = Utils::pythagoras(p.getPosition().x, p.getPosition().y, self->getPosition().x, self->getPosition().y) < 400;

        float move = self->getMovementSpeed() * self->getFlip() * 2 - 1;

        if (d->collidesWithDungeon({self->getPosition().x - move, self->getPosition().y}, DungeonEnemy::ENEMY_SIZE), false){
            // touched wall
            velocityY = -3.5;
        }else if (d->collidesWithDungeon({self->getPosition().x - move, self->getPosition().y + DungeonEnemy::ENEMY_SIZE.y}, DungeonEnemy::ENEMY_SIZE), false){
            self->getFlip() = !self->getFlip();
        }else {
            self->getPosition().x += move;
        }


        self->getPosition().y += velocityY;
        if (d->collidesWithDungeon({self->getPosition().x, self->getPosition().y + 1.0f}, DungeonEnemy::ENEMY_SIZE, true)){
            velocityY = 0;
        }else{
            velocityY += 0.1;
        }


        
    }
}