#pragma once
#include "interface.hpp"
#include <string.h>
#include <variant>
#include <iostream>
#include <vector>
#include "../resourceManager/entities.hpp"


namespace RandysEngine{
    
    constexpr std::size_t maxNodesMinitree = 16;
    constexpr std::size_t maxCamerasMinitree = 2;
    constexpr std::size_t maxModelsMinitree = 10;
    constexpr std::size_t maxLightsMinitree = 4;

    class layer_minitree : public layer_interface<layer_minitree>{

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
                RandysEngine::Pool::Static_pool_allocator<gl_matrix,32*4>
#else
                citro_matrix,
                RandysEngine::Pool::Static_pool_allocator<citro_matrix,32*4>
#endif
        >;


        SlotMapNodes nodes;
        SlotMapModels models;
        SlotMapLights lights;
        SlotMapCameras cameras;
        SlotMapMatrixes matrixes;

        SlotMap::SlotMap_Key rootNode;
        
        public:

            layer_minitree(ResourceManager& man);
            ~layer_minitree(){};

            [[nodiscard]] const RandysEngine::Layer_Node createNode() noexcept;

            [[nodiscard]] const RandysEngine::Layer_Node createNode(const RandysEngine::Layer_Node) noexcept;

            RandysEngine::MinitreeNode* getNode(const RandysEngine::Layer_Node) const noexcept;
            
            void addModel(RandysEngine::Layer_Node node) noexcept;

            RandysEngine::Model_Entity* getModel(RandysEngine::Layer_Node node) const noexcept;
            
            bool setTranslationMatrix(const RandysEngine::Layer_Node node,float x, float y, float z) const noexcept;
            bool setScalationMatrix(const RandysEngine::Layer_Node node,float x, float y, float z) const noexcept;
            bool setRotationMatrix(const RandysEngine::Layer_Node node,float x, float y, float z) const noexcept;

            bool TranslateMatrix(const RandysEngine::Layer_Node node,float x, float y, float z) const noexcept;
            bool ScaleMatrix(const RandysEngine::Layer_Node node,float x, float y, float z) const noexcept;
            bool RotateMatrix(const RandysEngine::Layer_Node node,float x, float y, float z) const noexcept;

            std::array<float,3> getTranslationMatrix(const RandysEngine::Layer_Node node) const noexcept;
            std::array<float,3> getScalationMatrix(const RandysEngine::Layer_Node node) const noexcept;
            std::array<float,3> getRotationMatrix(const RandysEngine::Layer_Node node) const noexcept;

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
            ) const;
            bool interact() const{
                return false;
            }
    };

}