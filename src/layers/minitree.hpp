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

        using SlotMapNodes = RandysEngine::SlotMap::SlotMap<
                MinitreeNode,
                RandysEngine::Pool::Static_pool_allocator<MinitreeNode,32>
                >;

        using SlotMapModels = RandysEngine::SlotMap::SlotMap<
                Model_Entity,
                RandysEngine::Pool::Static_pool_allocator<Model_Entity,1024*4>
                >;

        using SlotMapLights = RandysEngine::SlotMap::SlotMap<
                Light_Entity,
                RandysEngine::Pool::Static_pool_allocator<Light_Entity,128*4>
                >;

        using SlotMapCameras = RandysEngine::SlotMap::SlotMap<
                Camera_Entity,
                RandysEngine::Pool::Static_pool_allocator<Camera_Entity,8*4>
                >;

        SlotMapNodes nodes;
        SlotMapModels models;
        SlotMapLights lights;
        SlotMapCameras cameras;

        SlotMap::SlotMap_Key rootNode;
        ResourceManager::KeyId triangle_Mesh;

        bool activated {true};

        public:

            layer_minitree(ResourceManager& man) 
                : layer_interface<layer_minitree>(man), nodes{maxNodesMinitree},
                    models{maxModelsMinitree}, lights{maxLightsMinitree}, cameras{maxCamerasMinitree}{
                MinitreeNode e_rootNode;
                rootNode = nodes.push_back(e_rootNode);
                triangle_Mesh = man.createResource<gl_mesh_resource>("");
            };
            ~layer_minitree(){};

            bool addModel();

            void activate(){
                activated = true;
            };
            void deactivate(){
                activated = false;
            };
            bool draw() const;
            bool interact() const{
                return false;
            }
    };

}