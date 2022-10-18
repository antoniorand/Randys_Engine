#include "core.hpp"

int main(){

    RandysEngine::Rendering_Engine renderer;

    renderer.addLayerBack<RandysEngine::layer_GUI>();

    auto node1 = renderer.createNode<RandysEngine::layer_GUI>(0);
    if(renderer.addSprite(node1,10.0f,10.0f))
        std::cout << "YATAZO!\n";

    if(renderer.addTextureSprite(node1,"resources/rowlet.png"))
        std::cout << "YAROUUU!\n";


    while(renderer.isAppRunning()){
        if(renderer.readKeyPressed(RandysEngine::KeyInput::exit)){
            renderer.closeApp();
        }

        renderer.runFrame();
    }

    return 0;

}
