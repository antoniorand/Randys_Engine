#include "core.hpp"

class Demo{
    RandysEngine::Rendering_Engine renderer;
    
    RandysEngine::Layer_Node camera;
    std::vector<RandysEngine::Layer_Node> nodes;

    public:

    Demo(){
        nodes.reserve(50);
        renderer.addLayerBack<RandysEngine::layer_skybox>();
        renderer.addLayerBack<RandysEngine::layer_minitree>();
    }
    ~Demo(){};

    void addElement(std::string objectFile, std::string textureFile,
    float x, float y, float z){
        auto node = renderer.createNode<RandysEngine::layer_minitree>(1);
        renderer.addModel<RandysEngine::layer_minitree>(node);
        #ifndef __3DS__
            unsigned int meshN = renderer.addMesh<RandysEngine::layer_minitree>(node,"resources/"+ objectFile);
            renderer.addTexture<RandysEngine::layer_minitree>(node,meshN,"resources/" + textureFile + ".png");
        #else
            unsigned int meshN = renderer.addMesh<RandysEngine::layer_minitree>(node,"romfs:/3d_resources/"+ objectFile);
            renderer.addTexture<RandysEngine::layer_minitree>(node,meshN,"romfs:/gfx/" + textureFile + ".t3x");
        #endif
        renderer.setTranslateNode(node,x,y,z);
    }

    void addCamera(float x, float y, float z){
        camera = renderer.createNode<RandysEngine::layer_minitree>(1);
        renderer.addCamera<RandysEngine::layer_minitree>(camera,45.0f,400.0f,240.0f,0.01f,10000.0f);
        renderer.setActiveCamera<RandysEngine::layer_minitree>(camera);
        renderer.setTranslateNode(camera,x,y,z);
    }

    void addTextureSKybox(std::string fileName){
#ifndef __3DS__
        renderer.addTextureSkybox(0,"resources/" + fileName + ".png");
#else
        renderer.addTextureSkybox(0,"romfs:/gfx/" +  fileName + ".t3x");
#endif
    }

    void mainLoop(){
        while(renderer.isAppRunning()){
            if(renderer.readKeyPressed(RandysEngine::KeyInput::exit)){
                renderer.closeApp();
            }
            if(renderer.readKeyPressed(RandysEngine::KeyInput::left)){
                
                //renderer.TranslateNode(node0,-0.1f,0.0f,0.0f);
                //renderer.rotateCameraSKybox(2,0.0,-0.01,0.0);
            }
            if(renderer.readKeyPressed(RandysEngine::KeyInput::right)){
                //renderer.TranslateNode(node0,0.1f,0.0f,0.0f);
                //renderer.rotateCameraSKybox(2,0.0,0.01,0.0);
            }
            if(renderer.readKeyPressed(RandysEngine::KeyInput::up)){
                //renderer.RotateNode(node0,0.0f,-0.1f,0.0f);
                //renderer.rotateCameraSKybox(2,-0.01,0.0,0.0);
            }
            if(renderer.readKeyPressed(RandysEngine::KeyInput::down)){
                //renderer.RotateNode(node0,0.0f,0.1f,0.0f);
                //renderer.rotateCameraSKybox(2,0.01,0.0,0.0);
            }

            renderer.runFrame();
        }
    }
};

int main(){

    Demo demo;

    demo.addElement("road.obj","Pavimento_text",0.0f,0.0f,0.0f);

    demo.addCamera(0.0f,0.0f,0.0f);

    demo.addTextureSKybox("CubeExample");

    demo.mainLoop();    

    return 0;

}
