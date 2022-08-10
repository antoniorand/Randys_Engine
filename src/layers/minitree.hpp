#pragma once
#include "interface.hpp"
#include <string.h>
#include <variant>
#include <iostream>
#include <vector>
#include "../resourceManager/entities.hpp"

#ifndef __3DS__
    #include <glm/glm.hpp>
#endif
namespace RandysEngine{
    
    constexpr std::size_t maxNodesMinitree = 16;
    constexpr std::size_t maxCamerasMinitree = 16;
    constexpr std::size_t maxModelsMinitree = 16;
    constexpr std::size_t maxLightsMinitree = 16;

    class layer_minitree : public layer_interface<layer_minitree>{

        using VectorNodes = std::vector<
                ResourceManager::KeyId,
                RandysEngine::Pool::Static_pool_allocator<ResourceManager::KeyId,1024*4>
                >;

        using VectorModels = std::vector<
                ResourceManager::KeyId,
                RandysEngine::Pool::Static_pool_allocator<ResourceManager::KeyId,1024*4>
                >;

        using VectorLights = std::vector<
                ResourceManager::KeyId,
                RandysEngine::Pool::Static_pool_allocator<ResourceManager::KeyId,1024*4>
                >;

        using VectorCameras = std::vector<
                ResourceManager::KeyId,
                RandysEngine::Pool::Static_pool_allocator<ResourceManager::KeyId,1024*4>
                >;

        VectorNodes nodes;
        VectorModels models;
        VectorLights lights;
        VectorCameras cameras;

        ResourceManager::KeyId rootNode;

        bool activated {true};

        public:

            layer_minitree(ResourceManager& resource_Manager) 

                : layer_interface<layer_minitree>(resource_Manager){
                nodes.reserve(maxNodesMinitree);
                models.reserve(maxNodesMinitree);
                lights.reserve(maxNodesMinitree);
                cameras.reserve(maxNodesMinitree);
                {
                    MinitreeNode e_rootNode;

                }
            };
            ~layer_minitree(){};
            bool addButton(){
                //TODO
                return true;
            };
            bool addPicture(){
                //TODO
                return true;
            }

            bool addModel();

            void activate(){
                activated = true;
            };
            void deactivate(){
                activated = false;
            };
            bool draw(ResourceManager& man){
                bool devolver = true;
                if(!activated){
                    std::cout << "Cannot draw deactivated layer\n";
                    devolver = false;
                }
                else{
                }
                return devolver;
            };
            bool interact(){
                return false;
            }
    };

}