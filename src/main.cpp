#include "core.hpp"

constexpr double averageTick = 1/60.0;
int main(){

	RandysEngine::Rendering_Engine renderer;

    renderer.addLayerBack<RandysEngine::layer_minitree>();
    
    auto node1 = renderer.createNode<RandysEngine::layer_minitree>(0);

    auto node2 = renderer.createNode<RandysEngine::layer_minitree>(node1);

    renderer.addModel<RandysEngine::layer_minitree>(node2);

    unsigned int meshN = renderer.addMesh<RandysEngine::layer_minitree>(node2,"triangle");

    renderer.addTexture<RandysEngine::layer_minitree>(node2,meshN,"resources/face.jpg");

    //renderer.setTranslateNode(node2,0.0f,-1.0f,0.0f);

    while(renderer.isAppRunning()){
        if(renderer.readKeyPressed(RandysEngine::KeyInput::exit)){
            renderer.closeApp();
        }
        renderer.runFrame();
    }

    return 0;

}
