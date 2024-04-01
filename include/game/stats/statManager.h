#ifndef STAT_MANAGER
#define STAT_MANAGER 

#include <map>
#include "stat.h"
#include <vector>


namespace PlayerStats{
    
    
    class StatManager{
        private:
            std::map<int, Stat*> stats;
            static StatManager* instance;

        public:
            static StatManager* get();
            StatManager();
            ~StatManager();
            void resetAll();
            int pickRandomStat(int randomBase, std::vector<int>& excludeStats);
            int pickRandomStatWithWeightLessThan(int randomBase, std::vector<int>& excludeStats);
            Stat& getStat(int statId);
            int get(int statId);
            float getF(int statId); 
    };

    

}



#endif