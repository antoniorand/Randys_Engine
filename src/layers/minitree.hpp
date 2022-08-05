#pragma once
#include "interface.hpp"
#include <string.h>
#include <variant>
#include <iostream>
#ifndef __3DS__
    #include <glm/glm.hpp>
#endif
namespace RandysEngine{

    constexpr std::size_t maxNodesMinitree = 16;

    struct Model{int i;};
    struct Light{int i;};
    struct Camera{int i;};

    struct layer_minitree : public layer_interface{

        std::array<Model,maxNodesMinitree> models;
        std::array<Light,maxNodesMinitree> lights;
        std::array<Camera,maxNodesMinitree> cameras;

        
        struct MinitreeNode{
            std::variant<Model,Light,Camera> element;

#ifndef __3DS__
            
#else

#endif
        };

        bool activated {true};

        public:
            layer_minitree(){};
            ~layer_minitree(){};
            bool addButton(){
                //TODO
                return true;
            };
            bool addPicture(){
                //TODO
                return true;
            }
            void activate() override{
                activated = true;
            };
            void deactivate() override{
                activated = false;
            };
            bool draw() override{
                bool devolver = true;
                if(!activated){
                    std::cout << "Cannot draw deactivated layer\n";
                    devolver = false;
                }
                else{
                }
                return devolver;
            };
            bool interact() override{
                return false;
            }
    };

}