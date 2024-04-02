#include "game/stats/stat.h"
#include <cmath>


namespace PlayerStats {
    Stat::Stat(int defaultValue, int maxValue, int baseValue, float pointValue, std::string textName){
        this->defaultValue = defaultValue;
        this->maxValue = maxValue;
        this->baseValue = baseValue;
        this->isSutractive = true;
        this->value = defaultValue;
        this->pointValue = pointValue;
        this->textName = textName;
    }

    void Stat::resetStat(){
        this->value = this->defaultValue;
    }

    int Stat::getValueInBounds(){
        return std::max(std::min(value, maxValue), 1);
    }
    int Stat::get(){
        return baseValue + (int)std::ceil(getValueInBounds() * pointValue);
    }

    float Stat::getF(){
        return baseValue + (getValueInBounds() * pointValue);
    }

    void Stat::addToValue(int value){
        this->value += value;
    }
    

    std::string& Stat::getTextName(){
        return this->textName;
    }

    bool Stat::isUpgradable(){
        return true;
    }


}