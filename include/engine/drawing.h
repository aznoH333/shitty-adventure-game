#ifndef DRAWING 
#define DRAWING 

#include <string>
#include <map>
#include <queue>
#include <raylib-cpp.hpp>


const float DEFAULT_CAMERA_ZOOM = 3.0f;

struct RenderData{
    Texture2D* texture;
    Vector2 pos;
    float scale;
    Color color;
    float rotation;
    bool flipSprite;
};


enum DrawingLayers{
    LAYER_WORLD = 0,
    LAYER_OBJECT,
    LAYER_PLAYER,

};


class Drawing{



    private:
        static Drawing* instance;




        std::map<std::string, Texture2D> textures;
        std::map<int, std::queue<RenderData>> renderQueue;
        RenderTexture2D targetTexture;
        Camera2D camera;
        int renderLayerCount;

        const int gameScreenWidth = 1280;
        const int gameScreenHeight = 720;
        float screenScale = 1;



        void loadTexture(std::string filePath);
        void loadAllTexturesFromDirectory(std::string directoryPath);

    public:
        static Drawing* get();
        static void init(std::string assetsFolder, int renderLayerCount, int screenWidth, int screenHeight);
        void dispose();
        void render();
        Camera2D& getCamera();
        void drawTexture(std::string sprite, Vector2 pos, bool flipSprite, float scale, float rotation, Color color, int layer);




};


#endif