#include "game/dungeon/ai/fighterAi.h"
#include "game/dungeon/dungeon.h"


namespace EnemyAiCode{
    using namespace DungeonCode;

    void FighterAi::update(DungeonEnemy* self){
        DungeonPlayer& p = Dungeon::get()->getPlayer();


        bool isNearPlayer = Utils::pythagoras(p.getPosition().x, p.getPosition().y, self->getPosition().x, self->getPosition().y) < 400;

        if (Dungeon::get()->collidesWithDungeon({p.getPosition().x - 1, p.getPosition().y}, {16.0f, 16.0f})){
            
        }
    }
}