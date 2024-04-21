#ifndef STAT 
#define STAT 

#include <string>

namespace PlayerStats {

    enum StatName{
        SHELL_RELOAD_TIME = 0,
        AMMO_COUNT,
        MAX_AMMO_COUNT,
        FIRING_SPEED,
        PROJECTILE_COUNT,
        PROJECTILE_SPREAD,
        PROJECTILE_SPEED,
        PROJECTILE_DAMAGE,
        HEALTH,
        MAX_HEALTH,
        HEALING_POTION_COUNT,
        HEALING_POTION_MAX,
        STAT_COUNT,




    };
    
    class Stat{
        protected:
            const int MIN_STAT_VALUE = 1;

            int startingValue;
            int value;
            float multiplierPerPoint;
            
            int maxValue = 0;
            bool hasMaxValue = false;
            
            std::string textName;
            int getValueInBounds();
            bool upgradable = true;
        public:
            Stat(int startingValue, float multiplierPerPoint, std::string textName);

            virtual void resetStat();
            void addToValue(int value);
            virtual int get();
            virtual float getF();
            std::string& getTextName();
            virtual bool isUpgradable();
            void set(int value);
            Stat* disableUpgrading();
            Stat* declareMaxValue(int maxValue);
    };

}

#endif