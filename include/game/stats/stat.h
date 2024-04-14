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
        STAT_COUNT,



    };
    
    class Stat{
        protected:
            const int MIN_STAT_VALUE = 1;

            int value;
            int defaultValue;
            int baseValue;
            int maxValue;
            float pointValue;
            bool isSutractive;
            std::string textName;
            int getValueInBounds();
        public:
            Stat(int defaultValue, int maxValue, int baseValue, float pointValue, std::string textName);

            void resetStat();
            void addToValue(int value);
            virtual int get();
            virtual float getF();
            std::string& getTextName();
            virtual bool isUpgradable();
            void set(int value);
    };

}

#endif