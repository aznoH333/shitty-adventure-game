#include "game/stats/statVariable.h"


namespace PlayerStats {

    StatVariable::StatVariable(int defaultValue, std::string textName) : Stat(defaultValue, 99999, 0.0f, 0.0f, textName){

    }

    int StatVariable::get(){
        return value;
    }

    float StatVariable::getF(){
        return value;
    }

    bool StatVariable::isUpgradable(){
        return false;
    }
    

}
