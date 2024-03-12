#ifndef STAT 
#define STAT 

#include <climits> 

namespace PlayerStats {
    
    enum StatType{
        INT = 0,
        FLOAT,
    };

    enum StatWeight{
        LOW = 1,
        MED = 2,
        HIGH = 4,
        HIGHEST = 5,
        UNUPGRADABLE = INT_MAX,
    };

    enum StatName{
        SHELL_RELOAD_TIME_i = 0,
        AMMO_COUNT_i,
        MAX_AMMO_COUNT_i,
        FIRING_SPEED_i,
        PROJECTILE_COUNT_i,
       
       
        PROJECTILE_SPREAD_f,
        PROJECTILE_SPEED_f,
        PROJECTILE_DAMAGE_f,
        HEALTH_f,
        MAX_HEALTH_f,

        STAT_COUNT,


    };
    
    template<class T>
    class Stat{
        private:
            T value;
            T defaultValue;
            T averageValueChange;
            StatWeight weight;
        public:
            Stat<T>(T defaultValue, StatWeight weight, T averageValueChange);
            void resetStat();
            T& get();
            StatWeight getWeight();
            T getAverageValueChange();
    };

    template class Stat<int>;
    template class Stat<float>;

}

#endif