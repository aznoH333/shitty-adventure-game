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
        const int MAX_TEXT_LENGTH = 16;
        const int CHARACTER_WIDTH = 16;
        

        struct GameText{
            char textData[MAX_TEXT_LENGTH];
            int length = 0;
        };

        GameText stringToGameText(std::string input);
        void renderGameText(std::string text, Vector2 position, Color color, float size);
    };
    

};

#endif