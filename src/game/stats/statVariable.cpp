#include "game/stats/statVariable.h"


namespace PlayerStats {

    StatVariable::StatVariable(int defaultValue, std::string textName) : Stat(0, 0.0f, textName){
        this->originalValue = defaultValue;
        this->value = defaultValue;
    }

    void StatVariable::resetStat(){
        value = originalValue;
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
