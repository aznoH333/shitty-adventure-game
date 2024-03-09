#ifndef PLAYER_STATS


#define PLAYER_STATS


namespace PlayerStats {
    struct PlayerStats{
        
        int shellReloadTime = 15;
        int ammoCapacity = 4;
        int currentAmmoCount = 4;
        int firingSpeed = 20;
        int projectileCount = 4;
        float projectileSpread = 5.0f;
        float projectileSpeed = 6.0f;
    };

    
    // this sucks dick

    extern PlayerStats playerStats;


    void initDefault();
}


#endif