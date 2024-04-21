#ifndef STAT_VARIABLE
#define STAT_VARIABLE

#include <string>
#include "stat.h"

namespace PlayerStats {
    class StatVariable : public Stat{
        private:
            int originalValue;
        public:
            StatVariable(int defaultValue, std::string textName);
            
            void resetStat();
            int get();
            float getF();
            bool isUpgradable();

    };
}

#endif