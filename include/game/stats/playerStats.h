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
       
        // ty jsi hlupak wth proc si to delas
        FLOAT_START,
       
        PROJECTILE_SPREAD_f,
        PROJECTILE_SPEED_f,
        PROJECTILE_DAMAGE_f,
        HEALTH_f,
        MAX_HEALTH_f,

        FLOAT_END,

    };
    
    struct PlayerStats{
        StatManager<int> ints;
        StatManager<float> floats;
    };

    
    extern PlayerStats playerStats;

    void initDefault();
    void resetDefault();
    bool isStatFloat(int statId);
    int pickRandomValidStat(int seed);
}


#endif