#pragma once
#include "base_wrapper.hpp"
#include <3ds.h>
#include <citro3d.h>
#include <citro2d.h>
#include <vector>
#include "vshader_shbin.h"

#include <iostream>
#include <string>

namespace RandysEngine{

    class citro_skybox_helper : skybox_helper_wrapper<citro_skybox_helper>{

        public:
            citro_skybox_helper(){};

            void reverseCull() const noexcept;

            void restoreCull() const noexcept;
    };

    struct citro_matrix : public matrix_wrapper{

        private:

            C3D_Mtx transform;

        protected:

            friend struct citro_shader;
            const C3D_Mtx& getTransformationMatrix() noexcept;

        public:

            citro_matrix(){
                Mtx_Identity(&transform);
            }
            
            void multiply(citro_matrix& other);
            citro_matrix reverse() noexcept;
    };  

    struct citro_texture_resource : texture_resource_wrapper<citro_texture_resource>{

        C3D_Tex texture;

        C3D_Tex default_texture;

        citro_texture_resource(std::string file) noexcept;
        ~citro_texture_resource() noexcept;

        void use() noexcept;
        void unlink() noexcept;


    };

    struct citro_mesh_resource : mesh_resource_wrapper<citro_mesh_resource>{


        void* vbo_data;

        citro_mesh_resource(std::string file) noexcept;
        citro_mesh_resource(const citro_mesh_resource& other) noexcept{
            vbo_data = other.vbo_data;
        }
        citro_mesh_resource& operator=(const citro_mesh_resource& other) noexcept{
            vbo_data = other.vbo_data;
            return *this;
        }
       ~citro_mesh_resource() noexcept;
        void draw() const noexcept;

        private:
            std::vector<Vertex> loadModel(std::string file) noexcept;
    
    };

    struct citro_shader : shader_wrapper<citro_shader>{
        DVLB_s* vshader_dvlb;
        shaderProgram_s shaderProgram;

        citro_shader() noexcept;
       ~citro_shader() noexcept;

        void useShader() noexcept;
        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name, float value) const;
        void setMat4(const std::string &name, citro_matrix &mat);
    };

    struct citro_screen : screen_wrapper<citro_screen>{

        C3D_RenderTarget* target;
        
        bool running = true;

        citro_screen() noexcept;
       ~citro_screen() noexcept;

        void changeWindowSize(int e_width,int e_height) noexcept{};

        void swapBuffers() const noexcept;

        void clearDepth() const noexcept;

        bool getInputPressed(KeyInput input) const noexcept;

        bool isAppRunning() const noexcept;

        void closeApp() noexcept;

        void prepareDraw() const noexcept;

    };

    struct citro_main : initializer_wrapper<citro_main>{

        citro_main() noexcept;
       ~citro_main() noexcept;

    };

}

