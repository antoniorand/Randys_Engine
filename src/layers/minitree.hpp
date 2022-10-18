#pragma once
#include "interface.hpp"
#include <string.h>
#include <variant>
#include <iostream>
#include <vector>
#include "../resourceManager/entities.hpp"


namespace RandysEngine{
    
    class layer_minitree : public layer_interface<layer_minitree>{

        constexpr static std::size_t maxNodesMinitree = 16;
        constexpr static std::size_t maxCamerasMinitree = 2;
        constexpr static std::size_t maxModelsMinitree = 10;
        constexpr static std::size_t maxLightsMinitree = 4;

        using SlotMapNodes = RandysEngine::SlotMap::SlotMap<
                MinitreeNode,
                RandysEngine::Pool::Static_pool_allocator<MinitreeNode,32*4>
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

        using SlotMapMatrixes = RandysEngine::SlotMap::SlotMap<
#ifndef __3DS__
                gl_matrix,
                RandysEngine::Pool::Static_pool_allocator<gl_matrix,32*4 + 10*4>
#else
                citro_matrix,
                RandysEngine::Pool::Static_pool_allocator<citro_matrix,32*4 + 10*4>
#endif
        >;


        SlotMapNodes nodes;
        SlotMapModels models;
        SlotMapLights lights;
        SlotMapCameras cameras;
        SlotMapMatrixes matrixes;

        SlotMap::SlotMap_Key rootNode;

        RandysEngine::SlotMap::SlotMap_Key currentActiveCamera;
        bool isCameraActive; 

        
        void deleteLinkedNodes(SlotMap::SlotMap_Key);
        void runLinks(RandysEngine::MinitreeNode& node);
        
        void runLinkedMovement();

        void deleteEntity(RandysEngine::MinitreeNode& node);

        public:

            layer_minitree(ResourceManager& man);
            ~layer_minitree(){};

            [[nodiscard]] const RandysEngine::Layer_Node createNode() noexcept;

            [[nodiscard]] const RandysEngine::Layer_Node createNode(RandysEngine::Layer_Node&) noexcept;

            [[nodiscard]] bool deleteNode(RandysEngine::Layer_Node&) noexcept;
            
            void addModel(RandysEngine::Layer_Node& node) noexcept;
           
            void addCamera(RandysEngine::Layer_Node& node, float fov, float aspect, float near, float far) noexcept;
            bool setActiveCamera(RandysEngine::Layer_Node& node) noexcept;

            RandysEngine::Model_Entity* getModel(RandysEngine::Layer_Node& node) const noexcept;
            
            bool setTranslationMatrix(RandysEngine::Layer_Node& node,float x, float y, float z) const noexcept;
            bool setScalationMatrix(RandysEngine::Layer_Node& node,float x, float y, float z) const noexcept;
            bool setRotationMatrix(RandysEngine::Layer_Node& node,float x, float y, float z) const noexcept;

            bool TranslateMatrix(RandysEngine::Layer_Node& node,float x, float y, float z) const noexcept;
            bool ScaleMatrix(RandysEngine::Layer_Node& node,float x, float y, float z) const noexcept;
            bool RotateMatrix(RandysEngine::Layer_Node& node,float x, float y, float z) const noexcept;

            std::array<float,3> getTranslationMatrix(RandysEngine::Layer_Node& node) const noexcept;
            std::array<float,3> getScalationMatrix(RandysEngine::Layer_Node& node) const noexcept;
            std::array<float,3> getRotationMatrix(RandysEngine::Layer_Node& node) const noexcept;

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
            );


    };

}