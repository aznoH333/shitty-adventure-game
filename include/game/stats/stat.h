#ifndef STAT 
#define STAT 

#include <map>



namespace PlayerStats{
    
    
    template<class T>
    class StatManager{
        private:
            std::map<int, T> values;
            std::map<int, T> defaultValues;

        public:
            StatManager<T>();
            void resetValue(int index);
            void resetAll();
            void addValue(int index, T value);
            T& operator[](int index);

    };

    template class StatManager<int>;
    template class StatManager<float>;

}



#endif