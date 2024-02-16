#include "game/ui/ui.h"

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
        Drawing::get()->drawTextureStatic("door", {0.0f, y}, false, 1.0f, 0.0f, WHITE);
        
    }






    bool a = false;

    void updateUi(){
        UICode::Hud::updateHud();
        if (IsKeyPressed(KEY_U)){
            UICode::Hud::setHudVisibility(a);
            a = !a;
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
                    std::string name = "test_font_" + std::to_string(c);
                    Vector2 pos = {position.x + i * (CHARACTER_WIDTH * size), position.y};
                    
                    Drawing::get()->drawTextureStatic(name , pos, false, size, 0, color);
                }
            }
            
        }
    }



}