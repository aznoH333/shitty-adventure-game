#include "game/stats/stat.h"
#include <cmath>
#include <iostream>

namespace PlayerStats {
    Stat::Stat(int startingValue, float multiplierPerPoint, std::string textName){
        this->startingValue = startingValue;
        this->value = 0;
        this->multiplierPerPoint = multiplierPerPoint;
        this->textName = textName;
        this->upgradable = true;
    }

    void Stat::resetStat(){
        this->value = 0;
    }

    int Stat::getValueInBounds(){
        int out = std::max(0, value);
        if (hasMaxValue){
            out = std::min(value, maxValue);
        }
        return out;
    }
    
    int Stat::get(){
        return startingValue + (int)std::ceil(getValueInBounds() * multiplierPerPoint);
    }

    float Stat::getF(){
        return startingValue + (getValueInBounds() * multiplierPerPoint);
    }

    void Stat::addToValue(int value){
        this->value += value;
    }
    

    std::string& Stat::getTextName(){
        return this->textName;
    }

    bool Stat::isUpgradable(){
        return upgradable;
    }

    void Stat::set(int value){
        this->value = value;
    }

    Stat* Stat::disableUpgrading(){
        this->upgradable = false;
        return this;
    }

    Stat* Stat::declareMaxValue(int maxValue){
        this->maxValue = maxValue;
        this->hasMaxValue = true;
        return this;
    }



}