#ifndef STAT_MANAGER
#define STAT_MANAGER 

#include <map>
#include "stat.h"
#include <vector>


namespace PlayerStats{
    
    
    class StatManager{
        private:
            std::map<int, Stat<int>> intStats;
            std::map<int, Stat<float>> floatStats;
            static StatManager* instance;

        public:
            static StatManager* get();
            StatManager();
            void resetAll();
            int& getI(int index);
            float& getF(int index);
            int pickRandomStat(int randomBase, std::vector<int>& excludeStats);
            int pickRandomStatWithWeightLessThan(int randomBase, int weight, std::vector<int>& excludeStats);


    };

    

}



#endif