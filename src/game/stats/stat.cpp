#include "game/stats/stat.h"

namespace PlayerStats{
    template<typename T>
    StatManager<T>::StatManager(){
        values = std::map<int, T>();
        defaultValues = std::map<int, T>();
    }
    
    template<typename T>
    void StatManager<T>::resetValue(int index){
        values.at(index) = defaultValues.at(index);
    }
    
    template<typename T>
    T& StatManager<T>::operator[](int index){
        return values.at(index);
    }

    template<typename T>
    void StatManager<T>::addValue(int index, T value){
        defaultValues.emplace(index, value);
        values.emplace(index, value);
    }
    template<typename T>
    void StatManager<T>::resetAll(){
        for (std::pair<int, T> i : defaultValues){
            resetValue(i.first);
        }
    }
}