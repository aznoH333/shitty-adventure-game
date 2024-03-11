
#ifndef UTILS 
#define UTILS 

#include <memory>
#include <vector>
#include <string>
#include <filesystem>
#include <random>
#include <raylib.h>


namespace Utils{
    std::vector<std::string> getFilesInFolder(std::filesystem::path path);

    float interpolate(float a, float b, float w);
    float smoothstep(float value);
    float getRandomFloat();
    float getRandomFloat(float min, float max);
    float pythagoras(float x1, float y1, float x2, float y2);
    int relu(float a);
    float gravitateValue(float value, float goal, float stepSize);
    bool squaresCollide(Vector2 position1, Vector2 position2, Vector2 size1, Vector2 size2);

    const float RELU_PRECISION = 0.05f;

    extern int gameTickCounter;


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


    // strings and stuff for sprites
    int animationTimer(int start, int end, int frameTime);
    std::string animatedSprite(std::string& base, int start, int end, int frameDuration);
    std::string concatSprite(const char* base, int number);
    void drawLineOfSprites(const std::string& sprite, Vector2& startingPosition, int ammount, float spriteOffset);


    // --== ui utils ==--
    void drawBar(Vector2& position, const std::string& onSprite, const std::string& offSprite, float spriteOffset, int onAmmount, int ammount);


    // --== random array utils ==--
    template<typename T, typename A>
    bool isInVec(std::vector<T, A>& vec, T element);


}


#endif