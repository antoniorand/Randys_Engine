#pragma once
#include <string>
#include <cstdint>
#include "../APIs/openglGLAD.hpp"
namespace RandysEngine{

    
    struct Mesh;
    struct Vertex{
        float x, y, z;
    };
    struct Index;

    struct MODEL_DATA_GL{
        unsigned int VBO, VAO;
        MODEL_DATA_GL(const Vertex* input, unsigned int i){
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex), input, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
            glBindBuffer(GL_ARRAY_BUFFER, 0); 

            // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
            // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
            glBindVertexArray(0); 

        }
        ~MODEL_DATA_GL(){
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
        }
    };

    template <typename API_DATA>
    struct Model{


        API_DATA data;

        Model(std::string name){

        };
        Model(){
            Vertex left{-0.5f, -0.5f, 0.0f},
                right{0.5f, -0.5f, 0.0f},
                top{0.0f,  0.5f, 0.0f};
        };
        ~Model(){
            
        };
    }

};