#include "game/dungeon/dungeonBackground.h"
#include "engine/utils.h"
#include "engine/drawing.h"
#include "game/dungeon/dungeonTheme.h"

using namespace Utils;
namespace DungeonCode{

    void DungeonBackground::addLayer(BackgroundLayer layer){
        layers.push_back(layer);
    }

    DungeonBackground::DungeonBackground(){

    }

    void DungeonBackground::draw(float cameraOffset){
        
        // render backwards
        for (int i = layers.size() - 1 ; i >= 0; i--){
            layers.at(i).draw(cameraOffset);
        }
        
    }
    
    DungeonBackground DungeonBackground::generateBasedOnTheme(int themeId){
        DungeonBackground output = DungeonBackground();
        
        switch(themeId){
            case DungeonThemeType::THEME_DEFAULT:
                {
                    // layer 1
                    BackgroundLayer l1 = BackgroundLayer(0.25f);
                    for (int i = 0; i < 4; i++){
                        l1.addPattern("castle_background_1");
                        l1.addPattern("castle_background_2");
                    }
                    output.addLayer(l1);
                    // layer 2
                    BackgroundLayer l2 = BackgroundLayer(0.6f);
                    for (int i = 0; i < 10; i++){
                        l2.addPattern("castle_background_3");
                    }
                    output.addLayer(l2);

                    return output;
                }
            default:
                {
                    const std::vector<std::string> s = {"mud_background_1", "mud_background_2"};
                    BackgroundLayer layer = BackgroundLayer(0.4f);
                    for(int i = 0; i < 10; i++){
                        layer.addPattern(s.at(GetRandomValue(0, s.size() - 1)));
                    }
                    output.addLayer(layer);
                    return output;
                }
            
        }
    }

    void BackgroundLayer::draw(float cameraOffset){
        float b = (cameraOffset) * paralax;
        int a = std::floor(((cameraOffset - b - 200.0f)) / patternWidth) * patternWidth;
        for (int i = 0; i < contents.size(); i++){
            float x = (i * BACKGROUND_WIDTH) + a + b;
            float x2 = (((i + contents.size())) * BACKGROUND_WIDTH) + a + b;

            
            Drawing::get()->drawTexture(contents.at(i), {x, 0}, 0, 1.0f, 0.0f, WHITE, LAYER_BACKGROUND);
            Drawing::get()->drawTexture(contents.at(i), {x2, 0}, 0, 1.0f, 0.0f, WHITE, LAYER_BACKGROUND);
        }
    }

    void BackgroundLayer::addPattern(std::string pattern){
        contents.push_back(pattern);
        patternWidth = contents.size() * BACKGROUND_WIDTH;
    }

    BackgroundLayer::BackgroundLayer(float paralax){
        this->paralax = paralax;
    }
}