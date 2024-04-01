#include "game/stats/stat.h"
#include <cmath>


namespace PlayerStats {
    Stat::Stat(int defaultValue, int maxValue, float upperBound, float multiplierValue, std::string textName){
        this->defaultValue = defaultValue;
        this->maxValue = maxValue;
        this->upperBound = upperBound;
        this->isSutractive = true;
        this->value = defaultValue;
        this->multiplierValue = multiplierValue;
        this->textName = textName;
    }

    Stat::Stat(int defaultValue, int maxValue, float multiplierValue, std::string textName){
        this->defaultValue = defaultValue;
        this->maxValue = maxValue;
        this->upperBound = 0.0f;
        this->isSutractive = false;
        this->value = defaultValue;
        this->multiplierValue = multiplierValue;
        this->textName = textName;
    }
    void Stat::resetStat(){
        this->value = this->defaultValue;
    }

    int Stat::getValueInBounds(){
        return std::max(std::min(value, maxValue), 1);
    }
    int Stat::get(){
        if (isSutractive){
            return upperBound - (int)std::ceil(getValueInBounds() * multiplierValue);
        }
        
        return (int)std::ceil(getValueInBounds() * multiplierValue);
    }

    float Stat::getF(){
        if (isSutractive){
            return upperBound - (getValueInBounds() * multiplierValue);
        }
        
        return value * multiplierValue;
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