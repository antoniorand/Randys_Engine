#include "core.hpp"

int main(){

    RandysEngine::Rendering_Engine renderer;

    renderer.addLayerBack<RandysEngine::layer_GUI>();

    auto node1 = renderer.createNode<RandysEngine::layer_GUI>(0);
    if(renderer.addSprite(node1,0.0f,0.0f,0.0f,0.0f))
        std::cout << "YATAZO!\n";


    while(renderer.isAppRunning()){
        if(renderer.readKeyPressed(RandysEngine::KeyInput::exit)){
            renderer.closeApp();
        }

        renderer.runFrame();
    }

    return 0;

}
