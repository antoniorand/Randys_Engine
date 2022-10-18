#pragma once

#include <cstddef>
#include <string>
#include <array>


namespace RandysEngine{


    struct matrix_wrapper{

        std::array<float,3> translation{0.0f,0.0f,0.0f};
        std::array<float,3> rotation{0.0f,0.0f,0.0f};
        std::array<float,3> scalation{1.0f,1.0f,1.0f};

        float fov{0.0f}, aspect{0.0f}, near{0.0f}, far{0.0f};
        
        bool changed{true},perspective{false};

    };

    struct Vertex{
        float x, y, z; //Position
        float coordX, coordY; //Texture position

        Vertex(float input_x, float input_y, float input_z,
            float input_coordX, float input_coordY)
            : x{input_x}, y{input_y},z{input_z},coordX{input_coordX},coordY{input_coordY}{

        }
    };

    template<typename api>
    struct mesh_resource_wrapper{ 

        std::size_t size_loadedVertices{0}, count_loadedVertices{0};

        virtual ~mesh_resource_wrapper() noexcept{};

        void draw() const noexcept {
            static_cast<api*>(this)->draw();
        }

        protected:

            std::vector<Vertex> loadModel(std::string file) noexcept{
                return static_cast<api*>(this)->loadModel(file);
            }
    };

    template<typename api>
    struct texture_resource_wrapper{

        virtual ~texture_resource_wrapper() noexcept{};

        void use() noexcept{
            static_cast<api*>(this)->use();
        }
        void unlink() noexcept{
            static_cast<api*>(this)->unlink();
        }

        void setToSkybox() noexcept{
            static_cast<api*>(this)->setToSkyBox();
        }
    };

    template<typename api>
    struct shader_wrapper{

        virtual ~shader_wrapper() noexcept{};

        void useShader() noexcept{
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

    
    enum class KeyInput{
        exit,
        left,
        right,
        up,
        down,
        a_button,
        b_button,
        x_button,
        y_button
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

        void clearDepth() const noexcept{
            static_cast<api*>(this)->clearDepth();
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

    template<typename api>
    class skybox_helper_wrapper{

        public:
            skybox_helper_wrapper(){};

            void reverseCull() const noexcept{
                static_cast<api*>(this)->reverseCull();
            }

            void restoreCull() const noexcept{
                static_cast<api*>(this)->restoreCull();
            }
    };

}
