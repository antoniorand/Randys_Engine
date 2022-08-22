#pragma once
#include "base_wrapper.hpp"
#include "../GLAD/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

namespace RandysEngine{

    struct gl_matrix : matrix_wrapper<gl_matrix>{
        
    };  

    struct gl_mesh_resource : mesh_resource_wrapper<gl_mesh_resource>{

        const unsigned int indices[6] = {  // note that we start from 0!
            0, 1, 3,  // first Triangle
            1, 2, 3   // second Triangle
        };

        unsigned int VBO{0}, VAO{0},EBO{0};
        gl_mesh_resource(std::string file) noexcept;
        gl_mesh_resource(const gl_mesh_resource& other) noexcept{
            VBO = other.VBO;
            VAO = other.VAO;
        }
        gl_mesh_resource& operator=(const gl_mesh_resource& other) noexcept{
            VBO = other.VBO;
            VAO = other.VAO;
            return *this;
        }
       ~gl_mesh_resource() noexcept;
        void draw() const noexcept;
    
    };

    struct gl_shader : shader_wrapper<gl_shader>{

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

        gl_shader() noexcept;
       ~gl_shader() noexcept;

        void useShader() const noexcept;
    };

    struct gl_screen : screen_wrapper<gl_screen>{

        GLFWwindow* window;

        gl_screen() noexcept;
       ~gl_screen() noexcept;

        void changeWindowSize(int e_width,int e_height) noexcept;

        void swapBuffers() const noexcept;

        bool getInputPressed(KeyInput input) const noexcept;

        bool isAppRunning() const noexcept;
            

        void closeApp() noexcept;

        void prepareDraw() const noexcept;

    };

    struct gl_main : initializer_wrapper<gl_main>{

        gl_main() noexcept;
        

    };

}

