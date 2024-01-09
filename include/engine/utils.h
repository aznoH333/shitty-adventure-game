
#ifndef UTILS 
#define UTILS 

#include <vector>
#include <string>
#include <filesystem>
#include <random>


namespace Utils{
    std::vector<std::string> getFilesInFolder(std::filesystem::path path);

    float interpolate(float a, float b, float w);
    float getRandomFloat();
    float getRandomFloat(float min, float max);


    class SeededGenerator{
        private:
            std::random_device rd;
            std::mt19937* generator;
            std::uniform_real_distribution<>* distribution;
        public:
            SeededGenerator(int seed);
            ~SeededGenerator();
            float getRandomFloat();
            float getRandomFloat(float min, float max);
    };

}


#endif