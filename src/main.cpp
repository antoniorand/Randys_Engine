#include "core.hpp"

constexpr double averageTick = 1/60.0;
int main(){

	RandysEngine::Rendering_Engine renderer;

    renderer.addLayerBack<RandysEngine::layer_minitree>();
    renderer.addLayerBack<RandysEngine::layer_minitree>();
    
    auto node1 = renderer.createNode<RandysEngine::layer_minitree>(0);
    auto node2 = renderer.createNode<RandysEngine::layer_minitree>(node1);
    auto node3 = renderer.createNode<RandysEngine::layer_minitree>(node1);
    auto node4 = renderer.createNode<RandysEngine::layer_minitree>(1);

    auto nodeCamera = renderer.createNode<RandysEngine::layer_minitree>(node1);
    auto nodeCamera2 = renderer.createNode<RandysEngine::layer_minitree>(1);

    renderer.addModel<RandysEngine::layer_minitree>(node2);
    renderer.addModel<RandysEngine::layer_minitree>(node3);
    renderer.addModel<RandysEngine::layer_minitree>(node4);

    renderer.addCamera<RandysEngine::layer_minitree>(nodeCamera);
    renderer.setActiveCamera<RandysEngine::layer_minitree>(nodeCamera);

    renderer.addCamera<RandysEngine::layer_minitree>(nodeCamera2);
    renderer.setActiveCamera<RandysEngine::layer_minitree>(nodeCamera2);

#ifndef __3DS__
    unsigned int meshN = renderer.addMesh<RandysEngine::layer_minitree>(node2,"resources/cube.obj");
    unsigned int meshN2 = renderer.addMesh<RandysEngine::layer_minitree>(node3,"resources/rowlet.obj");
    unsigned int meshN3 = renderer.addMesh<RandysEngine::layer_minitree>(node4,"resources/rowlet.obj");
#else
    unsigned int meshN = renderer.addMesh<RandysEngine::layer_minitree>(node2,"romfs:/3d_resources/cube.obj");
    unsigned int meshN2 = renderer.addMesh<RandysEngine::layer_minitree>(node3,"romfs:/3d_resources/rowlet.obj");
    unsigned int meshN3 = renderer.addMesh<RandysEngine::layer_minitree>(node4,"romfs:/3d_resources/rowlet.obj");
#endif

#ifndef __3DS__
    renderer.addTexture<RandysEngine::layer_minitree>(node2,meshN,"resources/Cube.png");
    renderer.addTexture<RandysEngine::layer_minitree>(node3,meshN2,"resources/rowlet.png");
    renderer.addTexture<RandysEngine::layer_minitree>(node4,meshN3,"resources/rowlet.png");
#else
    renderer.addTexture<RandysEngine::layer_minitree>(node2,meshN,"romfs:/gfx/kitten.t3x");
    renderer.addTexture<RandysEngine::layer_minitree>(node3,meshN2,"romfs:/gfx/rowlet.t3x");
    renderer.addTexture<RandysEngine::layer_minitree>(node4,meshN3,"romfs:/gfx/rowlet.t3x");
#endif
    renderer.setTranslateNode(node2, 0.0f,0.0f,5.0f);
    renderer.setTranslateNode(node3, 1.0f,0.0f,-10.0f);
    renderer.setTranslateNode(node4, -1.0f,0.0f,10.0f);

    while(renderer.isAppRunning()){
        if(renderer.readKeyPressed(RandysEngine::KeyInput::exit)){
            renderer.closeApp();
        }
        if(renderer.readKeyPressed(RandysEngine::KeyInput::left)){
            renderer.TranslateNode(node2,-0.001f,0.0f,0.0f);
        }
        if(renderer.readKeyPressed(RandysEngine::KeyInput::right)){
            renderer.TranslateNode(node2,0.001f,0.0f,0.0f);
        }
        if(renderer.readKeyPressed(RandysEngine::KeyInput::up)){
            renderer.TranslateNode(node2,0.0f,0.0f,-0.001f);
        }
        if(renderer.readKeyPressed(RandysEngine::KeyInput::down)){
            renderer.TranslateNode(node2,0.0f,0.0f,0.001f);
        }

        renderer.runFrame();
    }

    return 0;

}
