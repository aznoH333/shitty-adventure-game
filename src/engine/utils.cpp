
#include "engine/utils.h"



namespace Utils {

    std::vector<std::string> getFilesInFolder(std::filesystem::path path){
        
        std::vector<std::string> output;

        for (const auto & entry : std::filesystem::directory_iterator(path)){

            if (entry.is_directory()){
                for (auto other : getFilesInFolder(entry.path())){
                    output.push_back(other);
                }
            }else {
                output.push_back(entry.path().string());
            }

        }


        return output;
    }


    float interpolate(float a, float b, float w){
        return (b - a) * w + a;
    }

    float smoothstep(float value){
        float x = value * value;
        float y = 1.0f - (1.0f - value) * (1.0f - value);
        
        return interpolate(x, y, value);
    }


    float pythagoras(float x1, float y1, float x2, float y2){
        float a = std::pow(std::abs(x1 - x2), 2);
        float b = std::pow(std::abs(y1 - y2), 2);

        return std::sqrt(a + b);
    }

    // --== generic random ==--
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_real_distribution<> distribution(0, 1);

    float getRandomFloat(){
        return distribution(generator);
    }

    float getRandomFloat(float min, float max){
        return getRandomFloat() * (max - min) + min;
    }

   
    
    // --== seeded random ==--
    SeededGenerator::SeededGenerator(int seed) {
        generator = new std::mt19937(seed);
        distribution = new std::uniform_real_distribution<>(0,1);
    };

    float SeededGenerator::getRandomFloat(){
        return (*distribution)(*generator);
    }

    float SeededGenerator::getRandomFloat(float min, float max){
        return getRandomFloat() * (max - min) + min;
    }

    int SeededGenerator::getRandomInt(int min, int max){
        return getRandomFloat(min, max);
    }

    SeededGenerator::~SeededGenerator(){
        delete generator;
        delete distribution;
    }


}

