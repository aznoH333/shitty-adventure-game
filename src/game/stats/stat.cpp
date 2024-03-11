#include "game/stats/stat.h"


namespace PlayerStats {
    template<typename T>
    Stat<T>::Stat(T defaultValue, StatWeight weight){
        this->defaultValue = defaultValue;
        this->weight = weight;
        this->value = defaultValue;
    }
    template<typename T>
    void Stat<T>::resetStat(){
        this->value = this->defaultValue;
    }

    template<typename T>
    T& Stat<T>::get(){
        return value;
    }

    template<typename T>
    StatWeight Stat<T>::getWeight(){
        return weight;
    }
}