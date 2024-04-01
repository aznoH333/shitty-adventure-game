#ifndef STAT_VARIABLE
#define STAT_VARIABLE

#include <string>
#include "stat.h"

namespace PlayerStats {
    class StatVariable : public Stat{
        public:
            StatVariable(int defaultValue, std::string textName);
            
            int get();
            float getF();
            bool isUpgradable();

    };
}

#endif