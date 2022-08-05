#pragma once
#include "interface.hpp"
#include <string.h>
#include <iostream>
#include <SOIL/SOIL.h>
#include "../APIs/generic/shaderOpengl.hpp"

namespace RandysEngine{

    class layer_skybox : public layer_interface{

        struct GraphicData{
            std::string skyboxFile[6];
            GLuint VAO, VBO, textureID;
        };

        GraphicData skybox {};
        bool activated {true};

        Shader skyboxShader{"shaders/skybox.vs", "shaders/skybox.fs"};

        bool loadTexture(){
            
            bool devolver = true;


            return devolver;
        }

        public:

            layer_skybox(){

                //create a cube and store it in the heap
                float skyboxVertices[108] = {
                    // positions          
                    -1.0f,  1.0f, -1.0f,
                    -1.0f, -1.0f, -1.0f,
                    1.0f, -1.0f, -1.0f,
                    1.0f, -1.0f, -1.0f,
                    1.0f,  1.0f, -1.0f,
                    -1.0f,  1.0f, -1.0f,

                    -1.0f, -1.0f,  1.0f,
                    -1.0f, -1.0f, -1.0f,
                    -1.0f,  1.0f, -1.0f,
                    -1.0f,  1.0f, -1.0f,
                    -1.0f,  1.0f,  1.0f,
                    -1.0f, -1.0f,  1.0f,

                    1.0f, -1.0f, -1.0f,
                    1.0f, -1.0f,  1.0f,
                    1.0f,  1.0f,  1.0f,
                    1.0f,  1.0f,  1.0f,
                    1.0f,  1.0f, -1.0f,
                    1.0f, -1.0f, -1.0f,

                    -1.0f, -1.0f,  1.0f,
                    -1.0f,  1.0f,  1.0f,
                    1.0f,  1.0f,  1.0f,
                    1.0f,  1.0f,  1.0f,
                    1.0f, -1.0f,  1.0f,
                    -1.0f, -1.0f,  1.0f,

                    -1.0f,  1.0f, -1.0f,
                    1.0f,  1.0f, -1.0f,
                    1.0f,  1.0f,  1.0f,
                    1.0f,  1.0f,  1.0f,
                    -1.0f,  1.0f,  1.0f,
                    -1.0f,  1.0f, -1.0f,

                    -1.0f, -1.0f, -1.0f,
                    -1.0f, -1.0f,  1.0f,
                    1.0f, -1.0f, -1.0f,
                    1.0f, -1.0f, -1.0f,
                    -1.0f, -1.0f,  1.0f,
                    1.0f, -1.0f,  1.0f
                };

          
            };
            ~layer_skybox(){

            };

            bool changeTexture(std::string right,std::string left,
                               std::string top,  std::string bottom, 
                               std::string front,std::string back){

                //store the source files
                skybox.skyboxFile[0] = right;
                skybox.skyboxFile[1] = left;
                skybox.skyboxFile[2] = top;
                skybox.skyboxFile[3] = bottom;
                skybox.skyboxFile[4] = front;
                skybox.skyboxFile[5] = back;
                //load the texture into opengl
                return loadTexture();

            }
            
            void activate() override{
                activated = true;
            };
            void deactivate() override{
                activated = false;
            };
            bool draw(Camera& activeCamera, ResourceManager& man) override{
                bool devolver = true;
                if(!activated){
                    std::cout << "Cannot draw deactivated layer\n";
                    devolver = false;
                }
                else{

                }
                return devolver;
            };
            bool interact() override{
                std::cout << "Cannot interact with skybox layer\n";
                return false;
            }
    };

}