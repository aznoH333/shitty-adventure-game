#ifndef UI
#define UI

#include "engine/utils.h"
#include "engine/drawing.h"
#include "game/stats/playerStats.h"
#include "game/stats/itemManager.h"
#include <string>

namespace UICode{
    
    
    
    void updateUi();

    namespace Hud {
        const float HUD_SPEED = 0.05f;
        const float HUD_OFF_POSITION = -64.0f;
        const float HUD_ON_POSITION = 0.0f;

        // shell counter
        const float SHELLS_OFFSET = 11.0f;
        const Vector2 SHELL_START = {4.0f, 4.0f};
        const std::string SHELL_SPRITE = "ammo_hud_1";
        const std::string SHELL_MISSING_SPRITE = "ammo_hud_2";

        // hp counter
        const float HP_OFFSET = 7.0f;
        const Vector2 HP_START = {4.0f, 27.0f};
        const float HEALTH_SEGMENT_COUNT = 10;
        const std::string HP_SPRITE = "hp_hud_1";
        const std::string HP_MISSING_SPRITE = "hp_hud_2";

        // item display
        const int DISPLAY_ITEM_COUNT = 5;
        const Vector2 ITEM_OFFSET = {2.0f, 2.0f};
        const float ITEM_FRAME_OFFSET = 19.0f;
        const Vector2 ITEM_DISPLAY_START = {300.0f, 4.0f};

        void setHudVisibility(bool state);
        void updateHud();
        void drawUi(float& hudY);
        void drawShells(float& hudY);
        void drawHealth(float& hudY);
        void drawItems(float& hudY);


        

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
            PlayerStats::Item* displayItem;
            float fadePercentage = 0.0f;
            std::string text = "";
            
            Vector2 position = {0.0f, 0.0f};
            Vector2 scale = {0.0f, 0.0f};
        };

        // Default text prompt
        const float DEFAULT_PROMPT_X = 109.0f;
        const float DEFAULT_PROMPT_Y = 156.0f;
        const int DEFAULT_PROMPT_WIDTH = 192;
        const int DEFAULT_PROMPT_HEIGHT = 64;
        const Vector2 DEFAULT_TEXT_POSITION = {117.0f, 182.0f};
        const float FADE_SPEED = 0.05f;

        // item display prompt
        const float ITEM_PROMPT_X = 93.0f;
        const float ITEM_PROMPT_Y = 20.0f;
        const float ITEM_PROMPT_WIDTH = 224.0f;
        const float ITEM_PROMPT_HEIGHT = 200.0f;
        const Vector2 ITEM_TEXT_POSITION = {117.0f, 52.0f};
        const float ITEM_TEXT_LINE_HEIGHT = 16.0f;
        const float ITEM_STAT_VALUE_START = 261.0f;
        const int SOME_ARBITRARY_VALUE = 9;


        const int BOX_SCALE = 32;
        void drawBox(Vector2 position, int width, int height, Color color);
        void displayDefaultTextPrompt(std::string text);
        void displayDismissPrompt(std::string text);
        void displayItemPrompt(PlayerStats::Item* itemToDisplay);
        void updateTextBoxes();
    };


    

};

#endif