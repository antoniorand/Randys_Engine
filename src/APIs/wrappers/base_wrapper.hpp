#pragma once

#include <cstddef>
#include <string>
#include <array>

//Source: https://github.com/Bly7/OBJ-Loader
#include <OBJ_LOADER.h>

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
    };

    template<typename api>
    struct mesh_resource_wrapper{ 

        static constexpr Vertex vertices[] = {
            {-0.5f, -0.5f, -0.5f,  0.0f, 0.0f},
            {0.5f, -0.5f, -0.5f,  1.0f, 0.0f},
            {0.5f,  0.5f, -0.5f,  1.0f, 1.0f},
            {0.5f,  0.5f, -0.5f,  1.0f, 1.0f},
            {-0.5f,  0.5f, -0.5f,  0.0f, 1.0f},
            {-0.5f, -0.5f, -0.5f,  0.0f, 0.0f},

            {-0.5f, -0.5f,  0.5f,  0.0f, 0.0f},
            {0.5f, -0.5f,  0.5f,  1.0f, 0.0f},
            {0.5f,  0.5f,  0.5f,  1.0f, 1.0f},
            {0.5f,  0.5f,  0.5f,  1.0f, 1.0f},
            {-0.5f,  0.5f,  0.5f,  0.0f, 1.0f},
            {-0.5f, -0.5f,  0.5f,  0.0f, 0.0f},

            {-0.5f,  0.5f,  0.5f,  1.0f, 0.0f},
            {-0.5f,  0.5f, -0.5f,  1.0f, 1.0f},
            {-0.5f, -0.5f, -0.5f,  0.0f, 1.0f},
            {-0.5f, -0.5f, -0.5f,  0.0f, 1.0f},
            {-0.5f, -0.5f,  0.5f,  0.0f, 0.0f},
            {-0.5f,  0.5f,  0.5f,  1.0f, 0.0f},

            {0.5f,  0.5f,  0.5f,  1.0f, 0.0f},
            {0.5f,  0.5f, -0.5f,  1.0f, 1.0f},
            {0.5f, -0.5f, -0.5f,  0.0f, 1.0f},
            {0.5f, -0.5f, -0.5f,  0.0f, 1.0f},
            {0.5f, -0.5f,  0.5f,  0.0f, 0.0f},
            {0.5f,  0.5f,  0.5f,  1.0f, 0.0f},

            {-0.5f, -0.5f, -0.5f,  0.0f, 1.0f},
            {0.5f, -0.5f, -0.5f,  1.0f, 1.0f},
            {0.5f, -0.5f,  0.5f,  1.0f, 0.0f},
            {0.5f, -0.5f,  0.5f,  1.0f, 0.0f},
            {-0.5f, -0.5f,  0.5f,  0.0f, 0.0f},
            {-0.5f, -0.5f, -0.5f,  0.0f, 1.0f},

            {-0.5f,  0.5f, -0.5f,  0.0f, 1.0f},
            {0.5f,  0.5f, -0.5f,  1.0f, 1.0f},
            {0.5f,  0.5f,  0.5f,  1.0f, 0.0f},
            {0.5f,  0.5f,  0.5f,  1.0f, 0.0f},
            {-0.5f,  0.5f,  0.5f,  0.0f, 0.0f},
            {-0.5f,  0.5f, -0.5f,  0.0f, 1.0f}
        };
        static constexpr unsigned short indices_list[] =
        {
            2,1,0,
            3,2,0
        };

        static constexpr std::size_t sizeVertices{sizeof(vertices)}, countVertices{sizeVertices/sizeof(vertices[0])};
        static constexpr std::size_t sizeIndices{sizeof(indices_list)}, countIndices{sizeIndices/sizeof(indices_list[0])}; 

        virtual ~mesh_resource_wrapper() noexcept{};

        void draw() const noexcept {
            static_cast<api*>(this)->draw();
        }

        protected:

            std::pair<
                std::vector<Vertex>,
                std::vector<unsigned short>>
            loadModel(std::string file){

                std::pair<
                    std::vector<Vertex>,
                    std::vector<unsigned short>
                > devolver;

                // Initialize Loader
                objl::Loader loader;

                // Load .obj File
                bool loadout = loader.LoadFile(file.c_str());

                // Check to see if it loaded

                // If so continue
                if (loadout){

                    const std::size_t nVertices = loader.LoadedVertices.size();
                    const std::size_t nIndices = loader.LoadedIndices.size();

                    devolver.first.reserve(nVertices);
                    devolver.second.reserve(nIndices);

                    for(unsigned int i = 0; i < nVertices;i++){
                        devolver.first.emplace_back(
                            loader.LoadedVertices[i].Position.X,
                            loader.LoadedVertices[i].Position.Y,
                            loader.LoadedVertices[i].Position.Z,
                            loader.LoadedVertices[i].TextureCoordinate.X,
                            loader.LoadedVertices[i].TextureCoordinate.Y
                        );
                    }
                    for(unsigned int i = 0; i < nIndices;i++){
                        devolver.second.push_back(
                            (unsigned short)loader.LoadedIndices[i]
                        );
                    }
                }

                return devolver;
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

    
    enum class KeyInput : unsigned int{
        exit = 0,
        left = 1,
        right = 2,
        up = 3,
        down = 4
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
