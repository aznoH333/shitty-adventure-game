#include "game/ui/ui.h"

namespace UICode{





    
    bool hudVisible = false;
    float hudProgress = 0;
        
    void Hud::setHudVisibility(bool state){
        if (state == hudVisible){
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
        Drawing::get()->drawTexture("door", {0.0f, y}, false, 1.0f, 0.0f, WHITE, LAYER_UI);
        
    }






    bool a = false;

    void updateUi(){
        UICode::Hud::updateHud();
        if (IsKeyPressed(KEY_U)){
            UICode::Hud::setHudVisibility(a);
            a = !a;
        }
    }



}