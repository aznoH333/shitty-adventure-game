#include "game/stats/playerStats.h"

namespace PlayerStats {
    StatManager* playerStats = 0;

    void initDefault(){
        playerStats = StatManager::get();
    }

    void dispose(){
        delete playerStats;
    }
}