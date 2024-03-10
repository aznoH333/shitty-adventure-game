#include "game/stats/playerStats.h"

namespace PlayerStats {
    PlayerStats playerStats = {
        StatManager<int>(),
        StatManager<float>()
    };



    void resetDefault(){
        playerStats.ints.resetAll();
        playerStats.floats.resetAll();
    }

    void initDefault(){
        // ints
        playerStats.ints.addValue(SHELL_RELOAD_TIME_i, 15);
        playerStats.ints.addValue(AMMO_COUNT_i, 4);
        playerStats.ints.addValue(MAX_AMMO_COUNT_i, 4);
        playerStats.ints.addValue(FIRING_SPEED_i, 20);
        playerStats.ints.addValue(PROJECTILE_COUNT_i, 4);

        // floats
        playerStats.floats.addValue(PROJECTILE_SPREAD_f, 5.0f);
        playerStats.floats.addValue(PROJECTILE_SPEED_f, 6.0f);
        playerStats.floats.addValue(PROJECTILE_DAMAGE_f, 9.0f);
        playerStats.floats.addValue(HEALTH_f, 100.0f);
        playerStats.floats.addValue(MAX_HEALTH_f, 100.0f);
    }
}