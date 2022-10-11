#include "core.hpp"

int main(){

    RandysEngine::Rendering_Engine renderer;

    renderer.addLayerBack<RandysEngine::layer_skybox>();
    renderer.addLayerBack<RandysEngine::layer_minitree>();
    
    auto node0  = renderer.createNode<RandysEngine::layer_minitree>(1);
    auto node1 = renderer.createNode<RandysEngine::layer_minitree>(node0);

    auto nodeCamera = renderer.createNode<RandysEngine::layer_minitree>(1);

    renderer.addModel<RandysEngine::layer_minitree>(node1);

    renderer.addCamera<RandysEngine::layer_minitree>(nodeCamera,45.0f,400.0f,240.0f,0.01f,10000.0f);
    renderer.setActiveCamera<RandysEngine::layer_minitree>(nodeCamera);


#ifndef __3DS__
    renderer.addTextureSkybox(0,"resources/CubeExample.png");
    unsigned int meshN = renderer.addMesh<RandysEngine::layer_minitree>(node1,"resources/test.obj");
    renderer.addTexture<RandysEngine::layer_minitree>(node1,meshN,"resources/test.png");
#else
    renderer.addTextureSkybox(0,"romfs:/gfx/CubeExample.t3x");
    unsigned int meshN = renderer.addMesh<RandysEngine::layer_minitree>(node1,"romfs:/3d_resources/test.obj");
    renderer.addTexture<RandysEngine::layer_minitree>(node1,meshN,"romfs:/gfx/test.t3x");
#endif

    renderer.setTranslateNode(node1, 0.0f,-1.0f,-15.0f);

    while(renderer.isAppRunning()){
        if(renderer.readKeyPressed(RandysEngine::KeyInput::exit)){
            renderer.closeApp();
        }
        if(renderer.readKeyPressed(RandysEngine::KeyInput::left)){
            
            renderer.TranslateNode(node1,-0.1f,0.0f,0.0f);
            //renderer.rotateCameraSKybox(2,0.0,-0.01,0.0);
        }
        if(renderer.readKeyPressed(RandysEngine::KeyInput::right)){
            renderer.TranslateNode(node1,0.1f,0.0f,0.0f);
            //renderer.rotateCameraSKybox(2,0.0,0.01,0.0);
        }
        if(renderer.readKeyPressed(RandysEngine::KeyInput::up)){
            renderer.RotateNode(node1,0.0f,-0.1f,0.0f);
            //renderer.rotateCameraSKybox(2,-0.01,0.0,0.0);
        }
        if(renderer.readKeyPressed(RandysEngine::KeyInput::down)){
            renderer.RotateNode(node1,0.0f,0.1f,0.0f);
            //renderer.rotateCameraSKybox(2,0.01,0.0,0.0);
        }
        if(renderer.readKeyPressed(RandysEngine::KeyInput::a_button)){
            //renderer.deleteNode<RandysEngine::layer_minitree>(node2);
        }


        renderer.runFrame();
    }

    return 0;

}
