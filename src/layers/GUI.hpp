#pragma once
#include "interface.hpp"
#include "../memoryPool/memoryPool.hpp"
#include <string.h>
#include <iostream>
namespace RandysEngine{

    constexpr std::size_t maxButtons = 10;
    constexpr std::size_t maxPictures = 10;

    class layer_GUI : public layer_interface<layer_GUI>{

        struct ButtonData{
            std::string ButtonName = "";
        };
        struct PictureData{
            std::string PictureName = "";
        };

        using buttonVector = std::vector<
            ButtonData, RandysEngine::Pool::Static_pool_allocator<ButtonData,maxButtons+1>
        >;

        using pictureVector = std::vector<
            PictureData, RandysEngine::Pool::Static_pool_allocator<PictureData,maxPictures+1>
        >;
    

        buttonVector buttons {};
        pictureVector pictures{}; 

        public:
            layer_GUI(ResourceManager& resource_Manager) 
                : layer_interface<layer_GUI>(resource_Manager){};
            ~layer_GUI(){};
            bool addButton(){
                //TODO
                return true;
            };
            bool addPicture(){
                //TODO
                return true;
            }
            void activate(){
                activated = true;
            };
            void deactivate(){
                activated = false;
            };
            bool draw(
#ifndef __3DS__
                RandysEngine::gl_shader* shader
#else
                RandysEngine::citro_shader* shader
#endif
            ) const{
                bool devolver = true;
                if(!activated){
                    std::cout << "Cannot draw deactivated layer\n";
                    devolver = false;
                }
                else{
                    std::cout << "Draw " << buttons.size() << " buttons \n";
                    std::cout << "Draw " << pictures.size() << " pictures \n";
                }
                return devolver;
            };
            bool interact() const{
                std::cout << "Interact with " << buttons.size() << " buttons \n";
                std::cout << "Interact with " << pictures.size() << " pictures \n";
                return false;
            }

            bool setTranslationMatrix(const RandysEngine::Layer_Node node, float x, float y, float z) const noexcept{
                return false;
            }
    };

}