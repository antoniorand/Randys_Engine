#pragma once
#include "interface.hpp"
#include <string.h>
#include <iostream>
namespace RandysEngine{

    class layer_skybox : public layer_interface{

        struct GraphicData{
            std::string skyboxFile = "";
        };

        GraphicData skybox {};
        bool activated {true};

        public:
            layer_skybox(){};
            ~layer_skybox(){};
            bool changeTexture(std::string input){
                skybox.skyboxFile = input;
                return true;
            };
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
                    std::cout << "Draw Skybox with file " << skybox.skyboxFile << std::endl;
                }
                return devolver;
            };
            bool interact() override{
                std::cout << "Cannot interact with skybox layer\n";
                return false;
            }
    };

}