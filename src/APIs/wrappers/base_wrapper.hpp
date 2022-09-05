#pragma once

#include <cstddef>
#include <string>
namespace RandysEngine{


    template<typename api>
    struct matrix_wrapper{

    };

    struct Vertex{
        float x, y, z; //Position
        float coordX, coordY; //Texture position
    };

    template<typename api>
    struct texture_resource_wrapper{

        bool loaded{false};
    
        virtual ~texture_resource_wrapper() noexcept{};

        void use() const noexcept;
        void unlink() const noexcept;
    };

    template<typename api>
    struct mesh_resource_wrapper{ 

        static constexpr Vertex vertices[] = {
            {0.0f,  0.6f, 0.0f, 1.0f, 0.0f}, //top left 
            {-0.5f, -0.6f, 0.0f, 0.0f, 0.0f}, //bottom left
            {0.5f,-0.6f, 0.0f, 1.0f, 0.0f}, //bottom right
            {1.0f, 0.6f, 0.0f, 1.0f, 1.0f} //top right
        };
        
        static constexpr unsigned short indices_list[] =
        {
            0,1,2,
            0,2,3
        };

        static constexpr std::size_t sizeVertices{sizeof(vertices)}, countVertices{sizeVertices/sizeof(vertices[0])};
        static constexpr std::size_t sizeIndices{sizeof(indices_list)}, countIndices{sizeIndices/sizeof(indices_list[0])}; 

        virtual ~mesh_resource_wrapper() noexcept{};

        void draw() const noexcept {
            static_cast<api*>(this)->draw();
        }
    };


    template<typename api>
    struct shader_wrapper{

        virtual ~shader_wrapper() noexcept{};

        void useShader() const noexcept{
            static_cast<api*>(this)->useShader();
        }

        // utility uniform functions
        void setBool(const std::string &name, bool value) const{
            static_cast<api*>(this)->setBool(name,value);
        }
        void setInt(const std::string &name, int value) const{
            static_cast<api*>(this)->setInt(name,value);
        }
        void setFloat(const std::string &name, float value) const{
            static_cast<api*>(this)->setFloat(name,value);
        }
    };

    template<typename api>
    struct initializer_wrapper {
        
        initializer_wrapper() noexcept{};
        virtual ~initializer_wrapper() noexcept{};

    };

    
    enum class KeyInput : unsigned int{
        exit = 0
    };

    template<typename api>
    struct screen_wrapper{

        int width {400},height{240};

        virtual ~screen_wrapper() noexcept{};

        void changeWindowSize(int e_width, int e_height) noexcept{
            static_cast<api*>(this)->changeWindowSize(e_width,e_height);
        }

        void swapBuffers() const noexcept{
            static_cast<api*>(this)->swapBuffers();
        }

        bool getInputPressed(KeyInput input) const noexcept{
            return static_cast<api*>(this)->getInput();
        }

        bool isAppRunning() const noexcept{
            return static_cast<api*>(this)->isAppRunning();
        }

        void closeApp() noexcept{
            return static_cast<api*>(this)->closeApp();
        }

        void prepareDraw() const noexcept{
            static_cast<api*>(this)->prepareDraw();
        }

    };

}
