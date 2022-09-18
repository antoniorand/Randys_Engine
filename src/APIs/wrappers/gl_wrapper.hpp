#pragma once
#include "base_wrapper.hpp"
#include "../GLAD/glad.h"
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>

namespace RandysEngine{

    //forward declaration
    struct gl_shader;

    struct gl_matrix : public matrix_wrapper{
        
        private:
            glm::mat4 transform{glm::mat4(1.0f)};

        protected: 

            friend struct gl_shader;
            const glm::mat4& getTransformationMatrix() noexcept;

    };  

    struct gl_texture_resource : texture_resource_wrapper<gl_texture_resource>{

        unsigned int texture;

        gl_texture_resource(std::string file) noexcept;

        void use() const noexcept;
        void unlink() const noexcept;

    };

    struct gl_mesh_resource : mesh_resource_wrapper<gl_mesh_resource>{

        unsigned int VBO{0}, VAO{0}, EBO{0};
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

        unsigned int shaderProgram;

        gl_shader() noexcept;
       ~gl_shader() noexcept;

        void useShader() const noexcept;
        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name, float value) const;
        void setMat4(const std::string &name, gl_matrix &mat) const;
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

