#ifndef PLAYER_STATS
#define PLAYER_STATS

#include <map>
#include "stat.h"

namespace PlayerStats {
    
    enum StatName{
        SHELL_RELOAD_TIME_i = 0,
        AMMO_COUNT_i,
        MAX_AMMO_COUNT_i,
        FIRING_SPEED_i,
        PROJECTILE_COUNT_i,
        PROJECTILE_SPREAD_f,
        PROJECTILE_SPEED_f,

    };
    
    struct PlayerStats{
        StatManager<int> ints;
        StatManager<float> floats;
    };

    
    // this sucks dick
    extern PlayerStats playerStats;

    void initDefault();
    void resetDefault();
}


#endif