
#include "engine/utils.h"
#include "engine/drawing.h"

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


    float gravitateValue(float value, float goal, float stepSize){
        float a = value + (Utils::relu(goal - value) * stepSize);
        if(std::abs(a - goal) > stepSize){
            return a;
        }else {
            return goal;
        }
    }

    float directionTowards(Vector2 start, Vector2 dest){
        return std::atan2(start.y - dest.y, start.x - dest.x) * RAD2DEG - 180;
    }

    float interpolate(float a, float b, float w){
        return (b - a) * w + a;
    }

    float smoothstep(float value){
        float x = value * value;
        float y = 1.0f - (1.0f - value) * (1.0f - value);
        
        return interpolate(x, y, value);
    }

    int boolToSign(bool input){
        return input * 2 - 1;
    }

    float pythagoras(float x1, float y1, float x2, float y2){
        float a = std::pow(std::abs(x1 - x2), 2);
        float b = std::pow(std::abs(y1 - y2), 2);

        return std::sqrt(a + b);
    }


    int relu(float a){
        return (a > RELU_PRECISION) + (a > -RELU_PRECISION) - 1;
    }


    bool squaresCollide(Vector2 position1, Vector2 position2, Vector2 size1, Vector2 size2){
        return 
            position1.x + size1.x > position2.x &&
            position1.x < position2.x + size2.x &&
            position1.y + size1.y > position2.y &&
            position1.y < position2.y + size2.y;
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



    // pseudo random
    const int PSEUDO_RANDOM_MAX = 4000;

    int hashInt(int input){
        input = ((input >> 16) ^ input) * 0x119de1f3;
        input = ((input >> 16) ^ input) * 0x119de1f3;
        input = (input >> 16) ^ input;
        return input;
    }

    int hashVector(int value1, int value2){
        return hashInt(hashInt(value1) + value2);
    }


    int getPseudoRandomInt(int min, int max, int base){
        int hashed = hashInt(base);
        return (hashed % ((max + 1) - min)) + min;
    }

    


    float getPseudoRandomFloat(int base){
        int baseInt = getPseudoRandomInt(0, PSEUDO_RANDOM_MAX, base);
        
        /*baseInt = baseInt & 0b10000000111111111111111111111111;
        baseInt = baseInt | 0b00111100000000000000000000000000;

        
            baseInt = baseInt & 0b10000000011111111111111111111111;
            baseInt = baseInt | 0b00111111000000000000000000000000;
        
        float res = (*(float*)(&baseInt));
        //float res = 0.baseInt;
        return res;*/

        return (float)baseInt / PSEUDO_RANDOM_MAX;
    }

    float getPseudoRandomFloat(float min, float max, int base){
        return getPseudoRandomFloat(base) * (max - min) + min;
    }

    // hacky workarounds
    int dividePosition(int position, int denominator){
        int negative = position < 0;
        int result = (position + negative) / denominator; 
        result -= negative;
        return result;
    }


    
    // --== sprites and other string functions ==--

    std::string animatedSprite(std::string& base, int start, int end, int frameDuration){
        return base + std::to_string(animationTimer(start, end, frameDuration));
    }

    int animationTimer(int start, int end, int frameTime){
        int time = gameTickCounter / frameTime;
        return start + (time % (end - start + 1));
    }

    std::string concatSprite(const char* base, int number){
        return base + std::to_string(number);
    }

    void drawLineOfSprites(const std::string& sprite, Vector2& startingPosition, int ammount, float spriteOffset){
        for (int i = 0; i < ammount; i++){
            Drawing::get()->drawTextureStatic(sprite, {startingPosition.x + i * spriteOffset, startingPosition.y}, false, 1.0f, 0.0f, WHITE);
        }
    }

    float sin(int time, float timeMultiplier, float valueMultiplier){
        return std::sin(time * timeMultiplier) * valueMultiplier;
    }


    // --== ui utils ==--
    void drawBar(Vector2& position, const std::string& onSprite, const std::string& offSprite, float spriteOffset, int onAmmount, int ammount){
        drawLineOfSprites(onSprite, position, onAmmount, spriteOffset);
        Vector2 pos = {position.x + (onAmmount * spriteOffset), position.y};
        drawLineOfSprites(offSprite, pos, ammount - onAmmount, spriteOffset);

    }


    // --== random array utils ==--
    template<typename T, typename A>
    bool isInVec(std::vector<T, A>& vec, T element){
        for (auto a : vec){
            if (element == a){
                return true;
            }
        }
        return false;
    }

    template bool isInVec<int, std::allocator<int>>(std::vector<int, std::allocator<int>>&, int);


}
