#pragma once
#include "interface.hpp"
#include "../memoryPool/memoryPool.hpp"
#include <string.h>
#include <iostream>
namespace RandysEngine{


    class layer_GUI : public layer_interface<layer_GUI>{

        
       constexpr static std::size_t maxNodes = 32;

        using SlotMapNodes = RandysEngine::SlotMap::SlotMap<
                GUINode,
                RandysEngine::Pool::Static_pool_allocator<GUINode,maxNodes*4>
        >;

        using SlotMapSprites = RandysEngine::SlotMap::SlotMap<
                Sprite_Entity,
                RandysEngine::Pool::Static_pool_allocator<Sprite_Entity,maxNodes*4>
        >;

        using SlotMapMatrixes = RandysEngine::SlotMap::SlotMap<
#ifndef __3DS__
            gl_matrix,
            RandysEngine::Pool::Static_pool_allocator<gl_matrix,maxNodes*4>
#else
            citro_matrix,
            RandysEngine::Pool::Static_pool_allocator<citro_matrix,maxNodes*4>
#endif
        >;
    
        SlotMapNodes nodes;
        SlotMapSprites sprites;
        SlotMapMatrixes matrixes;
        SlotMap::SlotMap_Key rootNode;

        Camera_GUI camera;

        public:
            layer_GUI(ResourceManager& resource_Manager);
            ~layer_GUI(){};

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

            [[nodiscard]] const RandysEngine::Layer_Node createNode() noexcept;

            [[nodiscard]] const RandysEngine::Layer_Node createNode(RandysEngine::Layer_Node& node);

            void addSprite(RandysEngine::Layer_Node& input, float width, float height) noexcept;

            RandysEngine::Sprite_Entity* getSprite(RandysEngine::Layer_Node&) const noexcept;

            bool setTranslationMatrix(RandysEngine::Layer_Node& node, float x, float y, float z) const noexcept{
                bool devolver = false;
                if(input.layerId == this->instance && input.isValid){

                    auto oldNode = nodes.atPosition(input.reference);
                    if(!oldNode){
                        input.isValid = false;
                    }
                    else{
                        auto& matrix_to_translate = *matrixes.atPosition(oldNode->matrixKey);

                        matrix_to_translate.translation[0] = x;
                        matrix_to_translate.translation[1] = y;
                        matrix_to_translate.changed = true;

                        devolver = true;
                    }

                }
                return devolver;
            }
            bool setScalationMatrix(RandysEngine::Layer_Node& node,float x, float y, float z) const noexcept{
                bool devolver = false;
                if(input.layerId == this->instance && input.isValid){

                    auto oldNode = nodes.atPosition(input.reference);
                    if(!oldNode){
                        input.isValid = false;
                    }
                    else{
                        auto& matrix_to_translate = *matrixes.atPosition(oldNode->matrixKey);

                        matrix_to_translate.scalation[0] = x;
                        matrix_to_translate.scalation[1] = y;
                        matrix_to_translate.changed = true;

                        devolver = true;
                    }

                }
                return devolver;
            }
            bool setRotationMatrix(RandysEngine::Layer_Node& node,float x, float y, float z) const noexcept{
                bool devolver = false;
                if(input.layerId == this->instance && input.isValid){

                    auto oldNode = nodes.atPosition(input.reference);
                    if(!oldNode){
                        input.isValid = false;
                    }
                    else{
                        auto& matrix_to_translate = *matrixes.atPosition(oldNode->matrixKey);

                        matrix_to_translate.rotation[0] = x;
                        matrix_to_translate.rotation[1] = y;
                        matrix_to_translate.changed = true;

                        devolver = true;
                    }

                }
                return devolver;
            }

            bool TranslateMatrix(RandysEngine::Layer_Node& node,float x, float y, float z) const noexcept{
                bool devolver = false;
                if(input.layerId == this->instance && input.isValid){

                    auto oldNode = nodes.atPosition(input.reference);

                    if(!oldNode){
                        input.isValid = false;
                    }
                    else{
                        auto& matrix_to_translate = *matrixes.atPosition(oldNode->matrixKey);

                        matrix_to_translate.translation[0] += x;
                        matrix_to_translate.translation[1] += y;
                        matrix_to_translate.changed = true;

                        devolver = true;
                    }


                }
                return devolver;
            }
            bool ScaleMatrix(RandysEngine::Layer_Node& node,float x, float y, float z) const noexcept{
                bool devolver = false;
                if(input.layerId == this->instance && input.isValid){

                    auto oldNode = nodes.atPosition(input.reference);

                    if(!oldNode){
                        input.isValid = false;
                    }
                    else{
                        auto& matrix_to_translate = *matrixes.atPosition(oldNode->matrixKey);

                        matrix_to_translate.scalation[0] += x;
                        matrix_to_translate.scalation[1] += y;
                        matrix_to_translate.changed = true;

                        devolver = true;
                    }


                }
                return devolver;
            }
            bool RotateMatrix(RandysEngine::Layer_Node& node,float x, float y, float z) const noexcept{
                bool devolver = false;
                if(input.layerId == this->instance && input.isValid){

                    auto oldNode = nodes.atPosition(input.reference);

                    if(!oldNode){
                        input.isValid = false;
                    }
                    else{
                        auto& matrix_to_translate = *matrixes.atPosition(oldNode->matrixKey);

                        matrix_to_translate.rotation[0] += x;
                        matrix_to_translate.rotation[1] += y;
                        matrix_to_translate.changed = true;

                        devolver = true;
                    }


                }
                return devolver;
            }

            std::array<float,3> getTranslationMatrix(RandysEngine::Layer_Node& node) const noexcept{
                return{0.0f,0.0f,0.0f};
            }
            std::array<float,3> getScalationMatrix(RandysEngine::Layer_Node& node) const noexcept{
                return{0.0f,0.0f,0.0f};
            }
            std::array<float,3> getRotationMatrix(RandysEngine::Layer_Node& node) const noexcept{
                return{0.0f,0.0f,0.0f};
            }
    };

}