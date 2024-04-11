#include "game/ui/ui.h"
#include <cmath>
#include "game/stats/itemManager.h"

using namespace Utils;
using namespace PlayerStats;

namespace UICode{





    
    bool hudVisible = false;
    float hudProgress = 0;
        
    void Hud::setHudVisibility(bool state){
        if (state == hudVisible || hudProgress < 0.99f){
            return;
        }
        
        hudVisible = state;
        hudProgress = 0;
    }
   
   
    // --== hud drawing ==--
    void Hud::drawUi(float& hudY){
        //Drawing::get()->drawTextureStatic("door", {0.0f, hudY}, false, 1.0f, 0.0f, WHITE);
        drawShells(hudY);
        drawHealth(hudY);
        drawItems(hudY);
    }


    void Hud::drawShells(float& hudY){
        Vector2 vec = {SHELL_START.x, SHELL_START.y + hudY};
        drawBar(vec, SHELL_SPRITE, SHELL_MISSING_SPRITE, SHELLS_OFFSET, playerStats->get(AMMO_COUNT), playerStats->get(MAX_AMMO_COUNT));
    }


    void Hud::drawHealth(float& hudY){
        Vector2 vec = {HP_START.x, HP_START.y + hudY};
        int healthSegments = (int)std::ceil(playerStats->getF(HEALTH) / playerStats->getF(MAX_HEALTH) * HEALTH_SEGMENT_COUNT);
        drawBar(vec, HP_SPRITE, HP_MISSING_SPRITE, HP_OFFSET, healthSegments, HEALTH_SEGMENT_COUNT);
    }

    void Hud::drawItems(float &hudY){
        for (int iterator = 0; iterator < DISPLAY_ITEM_COUNT; iterator++){
            Item* item = ItemManager::get()->getItem(ItemManager::get()->getItemInSlot(iterator));
            Vector2 drawPosition = {ITEM_DISPLAY_START.x + (iterator * ITEM_FRAME_OFFSET), ITEM_DISPLAY_START.y + hudY};
            
            
            Drawing::get()->drawTextureStatic("item_frame", drawPosition, false, 1.0f, 0.0f, WHITE);
            Drawing::get()->drawTextureStatic(item->getSprite(), {drawPosition.x + ITEM_OFFSET.x, drawPosition.y + ITEM_OFFSET.y}, false, 1.0f, 0.0f, WHITE);
        }
    }
   

   // --== hud update ==-
    void Hud::updateHud(){
        if (hudProgress < 1.0f){
            hudProgress += UICode::Hud::HUD_SPEED;
        }else {
            hudProgress = 1.0f;
        }
        float a = Utils::smoothstep(hudProgress); 
        float positionOffset = (UICode::Hud::HUD_ON_POSITION - UICode::Hud::HUD_OFF_POSITION) * a;
        float y;
        if (hudVisible){
            y = UICode::Hud::HUD_OFF_POSITION + positionOffset;
        }else {
            y = UICode::Hud::HUD_ON_POSITION - positionOffset;
        }
        
        // draw hud
        drawUi(y);
    }


    bool hudVisibility = false;

    void updateUi(){
        UICode::Hud::updateHud();
        UICode::Boxes::updateTextBoxes();
        if (IsKeyPressed(KEY_U)){
            UICode::Hud::setHudVisibility(hudVisibility);
            hudVisibility = !hudVisibility;
        }
    }



    /*
        SPECIAL char table
        -------------------
        a       |  coin
        b       |  key
        c       |  hp
        d       |  xp
        e       |  sp
        f       |  sword
        g       |  shield
        h       |  fire
        i       |  special
        j       |  talk
        k       |  buy
        l       |  leave

    
    */
    namespace Text {
        GameText stringToGameText(std::string input){
            GameText output;
            
            for (int iter = 0; iter < input.length(); iter++){

                char c = input[iter];

                // glyphs
                if (c == '('){
                    output.textData[output.length++] = input[++iter] - 'a' + 27;
                    iter++;
                
                
                
                
                // numbers
                }else if (c >= '0' && c <= '9'){
                    output.textData[output.length++] = c - '0' + 39;
                }
                
                
                // misc harcoded characters
                else if (c < 'a'){
                    switch(c){
                        case ' ':
                        default:
                            output.textData[output.length++] = 0;
                            break;
                        case '.':
                            output.textData[output.length++] = 49;
                            break;
                        case ':':
                            output.textData[output.length++] = 50;
                            break;
                        case '!':
                            output.textData[output.length++] = 51;
                            break;
                        case '?':
                            output.textData[output.length++] = 52;
                            break;
                        case '[':
                            output.textData[output.length++] = 53;
                            break;
                        case ']':
                            output.textData[output.length++] = 54;
                        case '+':
                            output.textData[output.length++] = 55;
                        case '-':
                            output.textData[output.length++] = 56;

                    }
                }
                
                
                // regular characters
                else {
                    output.textData[output.length++] = c - 'a' + 1;
                }

                if (output.length == MAX_TEXT_LENGTH){
                    break;
                }
            }

            return output;
        }


        void renderGameText(std::string text, Vector2 position, Color color, float size){
            GameText convertedText = stringToGameText(text);
            for (int i = 0; i < convertedText.length; i++){
                char c = convertedText.textData[i];

                if (c != 0){
                    std::string name = "font_" + std::to_string(c);
                    Vector2 pos = {position.x + i * (CHARACTER_WIDTH * size), position.y};
                    
                    Drawing::get()->drawTextureStatic(name , pos, false, size, 0, color);
                }
            }
            
        }
    }


    namespace Boxes {
        Prompt currentPrompt = {};
        
        void drawBox(Vector2 position, int width, int height, Color color){
            int boxWidth = width / BOX_SCALE;
            int boxHeight = height / BOX_SCALE;

            Drawing* d = Drawing::get();

            for (int x = 0; x < boxWidth; x++){
                for (int y = 0; y < boxHeight; y++){
                    
                    Vector2 drawPosition = {
                        position.x + (x * BOX_SCALE),
                        position.y + (y * BOX_SCALE)
                    };

                    // get correct sprite
                    int cornerSpriteIndex = 1;
                    
                    if (
                        (x == 0 || x == boxWidth - 1) && 
                        (y == 0 || y == boxHeight - 1)
                    ){
                        // hardcoded mess
                        cornerSpriteIndex += (y == 0) * 2;
                        cornerSpriteIndex += (x != 0);

                    }else {
                        cornerSpriteIndex = 5;
                    }

                    std::string sprite = "test_ui_box_" + std::to_string(cornerSpriteIndex);


                    d->drawTextureStatic(sprite, drawPosition, false, 1.0f, 0.0f, color);

                }
            }
        }


        void displayDefaultTextPrompt(std::string text){
            currentPrompt.isActive = true;
            currentPrompt.isVisible = true;
            currentPrompt.text = text;
            currentPrompt.displayItem = nullptr;

            currentPrompt.position = {DEFAULT_PROMPT_X, DEFAULT_PROMPT_Y};
            currentPrompt.scale = {DEFAULT_PROMPT_WIDTH, DEFAULT_PROMPT_HEIGHT};
        }


        void displayDismissPrompt(std::string text){


        }

        void updateTextBoxes(){
            if (!currentPrompt.isVisible){
                return;
            }
            // fade
            currentPrompt.fadePercentage = Utils::gravitateValue(currentPrompt.fadePercentage, (float)currentPrompt.isActive, FADE_SPEED);
            currentPrompt.isVisible = currentPrompt.fadePercentage > 0.05f;
            // calculate color
            Color boxColor = {255, 255, 255, 255};
            boxColor.a = (unsigned char)(currentPrompt.fadePercentage * 255);

            Color textColor = boxColor;
            boxColor.a /= 1.2f;


            drawBox(currentPrompt.position, currentPrompt.scale.x, currentPrompt.scale.y, boxColor);
            if (currentPrompt.displayItem == nullptr){
                // draw prompt
                UICode::Text::renderGameText(currentPrompt.text, DEFAULT_TEXT_POSITION, textColor, 0.7f);
            }else {
                // draw item display
                Item* item = currentPrompt.displayItem;
                // header
                UICode::Text::renderGameText(item->getName(), ITEM_TEXT_POSITION, textColor, 0.7f);
                // showcase stats
                int iteration = 1;
                for (std::pair<const int, int>& modifier : item->getModifierList()){
                    // i did this to myself
                    // i regret this
                    std::string text;
                    std::string value;
                    bool isPositive;
                    
                    
                    Stat& s = StatManager::get()->getStat(modifier.first);
                    text = s.getTextName();
                    value = std::to_string(modifier.second);
                    isPositive = modifier.second >= 0;
                    
                    
                    value = (isPositive ? "+" : "") + value;
                    // print
                    UICode::Text::renderGameText(text, {ITEM_TEXT_POSITION.x, ITEM_TEXT_POSITION.y + (ITEM_TEXT_LINE_HEIGHT * iteration)}, textColor, 0.7f);
                    UICode::Text::renderGameText(value, {ITEM_STAT_VALUE_START, ITEM_TEXT_POSITION.y + (ITEM_TEXT_LINE_HEIGHT * iteration)}, textColor, 0.7f);

                    iteration++;
                }
                // this code is genuine dog shit i hate c++
                UICode::Text::renderGameText("press e to pickup", {ITEM_TEXT_POSITION.x, ITEM_TEXT_POSITION.y + (ITEM_TEXT_LINE_HEIGHT * SOME_ARBITRARY_VALUE)}, textColor, 0.7f);
            }


            currentPrompt.isActive = false;
        }

        void displayItemPrompt(PlayerStats::Item* itemToDisplay){
            currentPrompt.isActive = true;
            currentPrompt.isVisible = true;
            //currentPrompt.text = text;
            currentPrompt.displayItem = itemToDisplay;
            currentPrompt.position = {ITEM_PROMPT_X, ITEM_PROMPT_Y};
            currentPrompt.scale = {ITEM_PROMPT_WIDTH, ITEM_PROMPT_HEIGHT};
        }
    }


}