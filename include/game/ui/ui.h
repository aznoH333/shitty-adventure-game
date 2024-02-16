#ifndef UI
#define UI

#include "engine/utils.h"
#include "engine/drawing.h"
#include <string>

namespace UICode{
    
    
    
    void updateUi();

    namespace Hud {
        const float HUD_SPEED = 0.05f;
        const float HUD_OFF_POSITION = -32.0f;
        const float HUD_ON_POSITION = 0.0f;


        void setHudVisibility(bool state);
        void updateHud();


        

    };

    namespace Text{
        const int MAX_TEXT_LENGTH = 32;
        const int CHARACTER_WIDTH = 16;
        

        struct GameText{
            char textData[MAX_TEXT_LENGTH];
            int length = 0;
        };

        GameText stringToGameText(std::string input);
        void renderGameText(std::string text, Vector2 position, Color color, float size);
    };

    namespace Boxes{

        struct Prompt{
            bool isVisible = false;
            bool isActive = false;
            bool dismissable = false;
            float fadePercentage = 0.0f;
            std::string text = "";
        };


        const float DEFAULT_PROMPT_X = 109.0f;
        const float DEFAULT_PROMPT_Y = 156.0f;
        const int DEFAULT_PROMPT_WIDTH = 192;
        const int DEFAULT_PROMPT_HEIGHT = 64;
        const Vector2 DEFAULT_TEXT_POSITION = {117.0f, 182.0f};
        const float FADE_SPEED = 0.05f;


        const int BOX_SCALE = 32;
        void drawBox(Vector2 position, int width, int height, Color color);
        void displayPrompt(std::string text);
        void displayDismissPrompt(std::string text);
        void updateTextBoxes();
    };
    

};

#endif