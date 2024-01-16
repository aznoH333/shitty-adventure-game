
#ifndef UTILS 
#define UTILS 

#include <vector>
#include <string>
#include <filesystem>
#include <random>


namespace Utils{
    std::vector<std::string> getFilesInFolder(std::filesystem::path path);

    float interpolate(float a, float b, float w);
    float smoothstep(float value);
    float getRandomFloat();
    float getRandomFloat(float min, float max);
    float pythagoras(float x1, float y1, float x2, float y2);

    static int gameTickCounter = 0;


    // psuedo random
    int getPseudoRandomInt(int min, int max, int base);
    float getPseudoRandomFloat(int base);
    float getPseudoRandomFloat(float min, float max, int base);


    // hacky workarounds
    int dividePosition(int position, int denominator);

    // hashing
    int hashInt(int input);
    int hashVector(int value1, int value2);

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
            int getRandomInt(int min, int max);
    };

}


#endif