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
            glGenTextures(1, &skybox.textureID);
            glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.textureID);

            int width, height, nrChannels;
            for (unsigned int i = 0; i < 6; i++){
                //unsigned char *data = stbi_load(skybox.skyboxFile[i].c_str(), &width, &height, &nrChannels, 0);
                auto data=SOIL_load_image(skybox.skyboxFile[i].c_str(),&width,&height,&nrChannels,SOIL_LOAD_RGBA);
    
                if (data){
                    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                    //stbi_image_free(data);
                    SOIL_free_image_data(data);
                }
                else{
                    std::cout << "Cubemap texture failed to load at path: " << skybox.skyboxFile[i] << std::endl;
                    //stbi_image_free(data);
                    devolver = false;
                }
            }
            if(devolver){
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            }
            else{
                glDeleteTextures(1,&skybox.textureID);
            }

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

                //generate vertex array
                glGenVertexArrays(1, &skybox.VAO);
                //generate VBO
                glGenBuffers(1, &skybox.VBO);
                //bind the vertex and the VBO we are going to work with
                glBindVertexArray(skybox.VAO);
                glBindBuffer(GL_ARRAY_BUFFER, skybox.VBO);
                //reserve data into the buffer
                glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);      

                skyboxShader.use();
                skyboxShader.setInt("skybox", 0);
          
            };
            ~layer_skybox(){
                //free memory
                glDeleteVertexArrays(1, &skybox.VAO);
                glDeleteBuffers(1, &skybox.VBO);
                glDeleteTextures(1,&skybox.textureID);
            };

            bool changeTexture(std::string right,std::string left,
                               std::string top,  std::string bottom, 
                               std::string front,std::string back){

                //delete texture, if any
                glDeleteTextures(1,&skybox.textureID);
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
            bool draw(Camera& activeCamera) override{
                bool devolver = true;
                if(!activated){
                    std::cout << "Cannot draw deactivated layer\n";
                    devolver = false;
                }
                else{
                    // draw skybox as last
                    //glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
                    skyboxShader.use();
                    auto view = glm::mat4(); 
                    auto projection = glm::mat4();
                    skyboxShader.setMat4("view", view);
                    skyboxShader.setMat4("projection", projection);
                    // skybox cube
                    glBindVertexArray(skybox.VAO);
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.textureID);
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                    glBindVertexArray(0);
                    glDepthFunc(GL_LESS); // set depth function back to default
                }
                return devolver;
            };
            bool interact() override{
                std::cout << "Cannot interact with skybox layer\n";
                return false;
            }
    };

}