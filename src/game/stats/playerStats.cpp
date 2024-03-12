#include "game/stats/playerStats.h"
#include "game/stats/itemManager.h"

namespace PlayerStats {
    StatManager* playerStats = 0;

    void initDefault(){
        playerStats = StatManager::get();
        ItemManager::get(); // lol mfao
    }

    void dispose(){
        delete playerStats;
    }
}