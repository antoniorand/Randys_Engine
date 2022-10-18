#pragma once
#include "interface.hpp"
#include <string.h>
#include <iostream>

namespace RandysEngine{

    class layer_skybox : public layer_interface<layer_skybox>{

        struct GraphicData{
            RandysEngine::ResourceManager::KeyId texture;
            bool hasTexture {false};

            RandysEngine::ResourceManager::KeyId skyboxCube;

            RandysEngine::Camera_Skybox camera;

        };

        GraphicData skybox {};

        public:

            layer_skybox(ResourceManager& resource_Manager);
            ~layer_skybox();

            void changeTexture(RandysEngine::ResourceManager::KeyId input);
            
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
            bool interact() const{
                //std::cout << "Cannot interact with skybox layer\n";
                return false;
            }

            void setRotationCameraSkyBox(float x, float y, float z) noexcept;
            void rotateCameraSkyBox(float x, float y, float z) noexcept;

            bool setTranslationMatrix(const RandysEngine::Layer_Node node, float x, float y, float z) const noexcept{
                return false;
            }
            bool setScalationMatrix(const RandysEngine::Layer_Node node,float x, float y, float z) const noexcept{
                return false;
            }
            bool setRotationMatrix(const RandysEngine::Layer_Node node,float x, float y, float z) const noexcept{
                return false;
            }

            bool TranslateMatrix(const RandysEngine::Layer_Node node,float x, float y, float z) const noexcept{
                return false;
            }
            bool ScaleMatrix(const RandysEngine::Layer_Node node,float x, float y, float z) const noexcept{
                return false;
            }
            bool RotateMatrix(const RandysEngine::Layer_Node node,float x, float y, float z) const noexcept{
                return false;
            }

            std::array<float,3> getTranslationMatrix(const RandysEngine::Layer_Node node) const noexcept{
                return{0.0f,0.0f,0.0f};
            }
            std::array<float,3> getScalationMatrix(const RandysEngine::Layer_Node node) const noexcept{
                return{0.0f,0.0f,0.0f};
            }
            std::array<float,3> getRotationMatrix(const RandysEngine::Layer_Node node) const noexcept{
                return{0.0f,0.0f,0.0f};
            }
    };

}