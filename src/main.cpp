#include "core.hpp"
#include <math.h>

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
    float x, float y, float z, float rotx, float roty, float rotz){
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
        renderer.setRotateNode(node,rotx,roty,rotz);
    }

    void addCamera(float x, float y, float z){
        camera = renderer.createNode<RandysEngine::layer_minitree>(1);
        renderer.addCamera<RandysEngine::layer_minitree>(camera,45.0f,400.0f,240.0f,0.01f,30000.0f);
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
                renderer.RotateNode(camera,0.0f,0.1f,0.0f);
                renderer.rotateCameraSKybox(0,0.0f,-0.1f,0.0f);
                std::cout << "left\n";
            }
            if(renderer.readKeyPressed(RandysEngine::KeyInput::right)){
                renderer.RotateNode(camera,0.0f,-0.1f,0.0f);
                renderer.rotateCameraSKybox(0,0.0f,0.1f,0.0f);
                std::cout<< "right\n";
            }
            if(renderer.readKeyPressed(RandysEngine::KeyInput::up)){
                //renderer.RotateNode(node0,0.0f,-0.1f,0.0f);
                //renderer.rotateCameraSKybox(2,-0.01,0.0,0.0);
            }
            if(renderer.readKeyPressed(RandysEngine::KeyInput::down)){
                //renderer.RotateNode(node0,0.0f,0.1f,0.0f);
                //renderer.rotateCameraSKybox(2,0.01,0.0,0.0);
            }
            if(renderer.readKeyPressed(RandysEngine::KeyInput::x_button)){
                //renderer.TranslateNode(camera,0.01f,0.01f,0.01f);
            }
            if(renderer.readKeyPressed(RandysEngine::KeyInput::y_button)){
                
            }
            if(renderer.readKeyPressed(RandysEngine::KeyInput::a_button)){
                
            }
            if(renderer.readKeyPressed(RandysEngine::KeyInput::b_button)){
                
            }

            renderer.runFrame();
        }
    }
};

int main(){

    Demo demo;

    demo.addElement("road.obj","Pavimento_text",0.0f,0.0f,0.0f,0.0f,0.0f,0.0f);
    /*demo.addElement("edificio.obj","Casa_text",14.0f,4.9f,36.0f,0.0f,0.0f,0.0f);
    demo.addElement("edificio.obj","Casa_text",23.0f,4.9f,36.0f,0.0f,0.0f,0.0f);
    demo.addElement("edificio.obj","Casa_text",33.0f,4.9f,36.0f,0.0f,0.0f,0.0f);
    demo.addElement("edificio.obj","Casa_text",42.0f,4.9f,36.0f,0.0f,0.0f,0.0f);
    demo.addElement("edificio.obj","Casa_text",52.0f,4.9f,36.0f,0.0f,0.0f,0.0f);
    demo.addElement("edificio.obj","Casa_text",61.0f,4.9f,36.0f,0.0f,0.0f,0.0f);
    demo.addElement("farola.obj","Farola_text",19.0f,0.0f,31.0f,0.0f,0.0f,0.0f);
    demo.addElement("farola.obj","Farola_text",30.0f,0.0f,31.0f,0.0f,0.0f,0.0f);
    demo.addElement("farola.obj","Farola_text",47.4f,0.0f,31.0f,0.0f,0.0f,0.0f);
    demo.addElement("farola.obj","Farola_text",66.8f,0.0f,31.0f,0.0f,0.0f,0.0f);
    demo.addElement("farola.obj","Farola_text",-7.8f,0.0f,28.0f,0.0f,0.0f,0.0f);
    demo.addElement("farola.obj","Farola_text",-15.18f,0.0f,10.35f,0.0f,0.0f,0.0f);
    demo.addElement("farola.obj","Farola_text",-15.18f,0.0f,0.0f,0.0f,0.0f,0.0f);*/

    demo.addCamera(0.0f,0.0f,0.0f);

    demo.addTextureSKybox("CubeExample");

    demo.mainLoop();    

    return 0;

}
