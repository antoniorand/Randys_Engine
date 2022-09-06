#include "core.hpp"

constexpr double averageTick = 1/60.0;
int main(){

	RandysEngine::Rendering_Engine renderer;
    renderer.addLayerBack<RandysEngine::layer_minitree>();
    auto& minitree = *renderer.getLayer<RandysEngine::layer_minitree>(0);

    auto key = minitree.addModel();

    while(renderer.isAppRunning()){
        if(renderer.readKeyPressed(RandysEngine::KeyInput::exit)){
            renderer.closeApp();
        }
        renderer.runFrame();
    }

    return 0;

}
