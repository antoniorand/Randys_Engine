#pragma once
#include "interface.hpp"
#include <string.h>
#include <iostream>
#include <stb/stb_image.h>
#include "../APIs/generic/shaderOpengl.hpp"
namespace RandysEngine{

    class layer_skybox : public layer_interface{

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

        struct GraphicData{
            std::string skyboxFile = "";
            unsigned int VAO, VBO, textureID;
            
        };

        GraphicData skybox {};
        bool activated {true};

        void loadTexture(){
            unsigned int textureID;
            glGenTextures(1, &textureID);

            int width, height, nrComponents;
            unsigned char *data = stbi_load(skybox.skyboxFile.c_str(), &width, &height, &nrComponents, 0);
            if (data){
                GLenum format;
                if (nrComponents == 1)
                    format = GL_RED;
                else if (nrComponents == 3)
                    format = GL_RGB;
                else if (nrComponents == 4)
                    format = GL_RGBA;

                glBindTexture(GL_TEXTURE_2D, textureID);
                glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                stbi_image_free(data);
                skybox.textureID = textureID;
            }
            else
            {
                std::cout << "Texture failed to load at path: " << skybox.skyboxFile << std::endl;
                stbi_image_free(data);
            }
        }

        public:
            layer_skybox(){
                glGenVertexArrays(1, &skybox.VAO);
                glGenBuffers(1, &skybox.VBO);
                glBindVertexArray(skybox.VAO);
                glBindBuffer(GL_ARRAY_BUFFER, skybox.VBO);
                glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);                
            };
            ~layer_skybox(){
                glDeleteVertexArrays(1, &skybox.VAO);
                glDeleteBuffers(1, &skybox.VBO);
            };
            bool changeTexture(std::string input){
                skybox.skyboxFile = input;
                
                return true;
            };
            void activate() override{
                activated = true;
            };
            void deactivate() override{
                activated = false;
            };
            bool draw() override{
                bool devolver = true;
                if(!activated){
                    std::cout << "Cannot draw deactivated layer\n";
                    devolver = false;
                }
                else{
                    std::cout << "Draw Skybox with file " << skybox.skyboxFile << std::endl;
                }
                return devolver;
            };
            bool interact() override{
                std::cout << "Cannot interact with skybox layer\n";
                return false;
            }
    };

}