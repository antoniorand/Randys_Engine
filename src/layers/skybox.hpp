#pragma once
#include "interface.hpp"
#include <string.h>
#include <iostream>

namespace RandysEngine{

    class layer_skybox : public layer_interface<layer_skybox>{

        struct GraphicData{
            std::string skyboxFile[6];
            //GLuint VAO, VBO, textureID;
        };

        GraphicData skybox {};

        //Shader skyboxShader{"shaders/skybox.vs", "shaders/skybox.fs"};

        bool loadTexture(){
            
            bool devolver = true;


            return devolver;
        }

        public:

            layer_skybox(ResourceManager& resource_Manager) 
                : layer_interface<layer_skybox>(resource_Manager){

                //create a cube and store it in the heap
                /*float skyboxVertices[108] = {
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
                };*/

          
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
            
            void activate(){
                activated = true;
            };
            void deactivate(){
                activated = false;
            };
            bool draw(
#ifndef __3DS__
                RandysEngine::gl_shader* shader
#else
                RandysEngine::citro_shader* shader
#endif
            ) const{
                bool devolver = true;
                if(!activated){
                    std::cout << "Cannot draw deactivated layer\n";
                    devolver = false;
                }
                else{

                }
                return devolver;
            };
            bool interact() const{
                std::cout << "Cannot interact with skybox layer\n";
                return false;
            }

            bool setTranslationMatrix(const RandysEngine::Layer_Node node, float x, float y, float z) const noexcept{
                return false;
            }
    };

}