#pragma once
#include "base_wrapper.hpp"
#include <3ds.h>
#include <iostream>
#include <string>

namespace RandysEngine{

    struct citro_matrix : matrix_wrapper<citro_matrix>{
        
    };  

    struct citro_mesh_resource : mesh_resource_wrapper<citro_mesh_resource>{
        unsigned int VBO{0}, VAO{0};
        citro_mesh_resource(std::string file) noexcept;
        citro_mesh_resource(const citro_mesh_resource& other) noexcept{
            VBO = other.VBO;
            VAO = other.VAO;
        }
        citro_mesh_resource& operator=(const citro_mesh_resource& other) noexcept{
            VBO = other.VBO;
            VAO = other.VAO;
            return *this;
        }
       ~citro_mesh_resource() noexcept;
        void draw() const noexcept;
    
    };

    struct citro_shader : shader_wrapper<citro_shader>{

        const char *vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

        const char *fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n\0";

        unsigned int shaderProgram;

        citro_shader();
       ~citro_shader();

        void useShader(){
            //glUseProgram(shaderProgram);
        }
    };

    struct citro_screen : screen_wrapper<citro_screen>{

        //GLFWwindow* window;

        C3D_RenderTarget* targetLeft;
        C3D_RenderTarget* targetRight;

        citro_screen();
       ~citro_screen();

        void changeWindowSize(int e_width,int e_height);

        void swapBuffers(){
            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            // -------------------------------------------------------------------------------
            //glfwSwapBuffers(window);
            //glfwPollEvents();
        }

        bool getInputPressed(KeyInput input) const{
            bool devolver = false;
            switch (input){
                case KeyInput::exit :
                //    if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
                    devolver = true;
                break;
                default: 
                break;
            }
            return devolver;
        }

        bool isAppRunning() const{
            //return !glfwWindowShouldClose(window);
            return true;
        }

        void closeApp(){
            //glfwSetWindowShouldClose(window, GL_TRUE);
        }

    };

    struct citro_main : initializer_wrapper<citro_main>{

        citro_main();

        void prepareDraw(){
            // render
            // ------
            //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            //glClear(GL_COLOR_BUFFER_BIT);            
        }

    };

}

