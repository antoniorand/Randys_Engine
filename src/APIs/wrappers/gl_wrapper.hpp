#pragma once
#include "base_wrapper.hpp"
#include "../GLAD/glad.h"
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>

namespace RandysEngine{

    class gl_matrix : matrix_wrapper<gl_matrix>{
        glm::mat4 transform{glm::mat4(1.0f)};

        bool changed{false};

        glm::vec3 translation {glm::vec3(0.0f)};
        glm::vec3 rotation {glm::vec3(0.0f)};
        glm::vec3 scalation {glm::vec3(0.0f)};

        private:

            void translate(float x_position, float y_position, float z_position) noexcept;

            void rotate(float x_rotation, float y_rotation, float z_rotation) noexcept;

            void scale(float x_scale, float y_scale, float z_scale) noexcept;

            float getTranslate_x() const noexcept;

            float getTranslate_y() const noexcept;

            float getTranslate_z() const noexcept;

            float getRotation_x() const noexcept;

            float getRotation_y() const noexcept;

            float getRotation_z() const noexcept;

            float getScale_x() const noexcept;

            float getScale_y() const noexcept;

            float getScale_z() const noexcept;

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

