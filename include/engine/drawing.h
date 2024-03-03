#ifndef DRAWING 
#define DRAWING 

#include <string>
#include <map>
#include <queue>
#include <raylib-cpp.hpp>


const float DEFAULT_CAMERA_ZOOM = 3.0f;
const int DEFAULT_FONT_SIZE = 20;

struct RenderData{
    Texture2D* texture;
    Vector2 pos;
    float scale;
    Color color;
    float rotation;
    int flipSprite;
};

struct TextRenderData{
    std::string text;
    Vector2 position;
    int scale;
    Color color;
};


enum DrawingLayers{
    LAYER_WORLD = 0,
    LAYER_OBJECT,
    LAYER_PROJECTILES,
    LAYER_ENEMY,
    LAYER_PLAYER,
    LAYER_EFFECTS,
    LAYER_UI,

};


class Drawing{



    private:
        static Drawing* instance;
        const Color backgroundColor = {33, 30, 32, 255};




        std::map<std::string, Texture2D> textures;
        std::map<int, std::queue<RenderData>> renderQueue;
        std::queue<RenderData> staticQueue;
        std::queue<TextRenderData> textQueue;
        RenderTexture2D targetTexture;
        Camera2D camera;
        Camera2D staticCamera;

        int renderLayerCount;

        const int gameScreenWidth = 1280;
        const int gameScreenHeight = 720;

        
        float screenScale = 1;
        int screenShake = 0;
        const int SCREEN_SHAKE_MAX = 20;



        void loadTexture(std::string filePath);
        void loadAllTexturesFromDirectory(std::string directoryPath);
        void renderText(TextRenderData& text);

    public:
        static Drawing* get();
        static void init(std::string assetsFolder, int renderLayerCount, int screenWidth, int screenHeight);
        void dispose();
        Vector2 getInworldMousePosition();
        void render();
        Camera2D& getCamera();
        void drawTexture(std::string sprite, Vector2 pos, int flipSprite, float scale, float rotation, Color color, int layer);
        void drawTextureStatic(std::string sprite, Vector2 pos, bool flipSprite, float scale, float rotation, Color color);
        void drawText(std::string text, Vector2 pos, float scale, Color color);
        void addScreenShaker(int ammount);




};


#endif