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

        using SlotMapModels = RandysEngine::SlotMap::SlotMap<
                ResourceManager::KeyId,
                RandysEngine::Pool::Static_pool_allocator<ResourceManager::KeyId,1024*4>
                >;

        using SlotMapLights = RandysEngine::SlotMap::SlotMap<
                ResourceManager::KeyId,
                RandysEngine::Pool::Static_pool_allocator<ResourceManager::KeyId,1024*4>
                >;

        using SlotMapCameras = RandysEngine::SlotMap::SlotMap<
                ResourceManager::KeyId,
                RandysEngine::Pool::Static_pool_allocator<ResourceManager::KeyId,1024*4>
                >;

        VectorNodes nodes;
        SlotMapModels models;
        SlotMapLights lights;
        SlotMapCameras cameras;

        ResourceManager::KeyId rootNode;
        ResourceManager::KeyId triangle_Mesh;

        bool activated {true};

        public:

            layer_minitree(ResourceManager& man) 
                : layer_interface<layer_minitree>(man), 
                    models{maxNodesMinitree}, lights{maxNodesMinitree}, cameras{maxNodesMinitree}{
                nodes.reserve(maxNodesMinitree);
                {
                    MinitreeNode e_rootNode;
                    gl_mesh_resource triangle{""};
                    rootNode = man.reserveResource<MinitreeNode>(e_rootNode);
                    triangle_Mesh = man.reserveResource<gl_mesh_resource>(triangle);
                }
            };
            ~layer_minitree(){};

            bool addModel();

            void activate(){
                activated = true;
            };
            void deactivate(){
                activated = false;
            };
            bool draw() const{
                bool devolver = true;
                if(!activated){
                    std::cout << "Cannot draw deactivated layer\n";
                    devolver = false;
                }
                else{
                }
                return devolver;
            };
            bool interact() const{
                return false;
            }
    };

}