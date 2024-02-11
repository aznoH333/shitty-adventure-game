#ifndef UI
#define UI

#include "engine/utils.h"
#include "engine/drawing.h"

namespace UICode{
    void updateUi();

    namespace Hud {
        const float HUD_SPEED = 0.05f;
        const float HUD_OFF_POSITION = -16.0f;
        const float HUD_ON_POSITION = 32.0f;

        void setHudVisibility(bool state);
        void updateHud();
    }
    

};

#endif