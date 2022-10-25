#pragma once
#include "resourceManager.hpp"

#ifndef __3DS__
    #include "../APIs/wrappers/gl_wrapper.hpp"
#else
    #include "../APIs/wrappers/citro_wrapper.hpp"
#endif

namespace RandysEngine{

    enum class entityType_enum{
        model,
        light,
        camera,
        none
    };

    struct Base_Entity{
        SlotMap::SlotMap_Key matrixKey;
    };

    struct Model_Entity : Base_Entity{

        static constexpr unsigned int MAXMESHES {4};

        std::array<ResourceManager::KeyId,MAXMESHES> meshes;
        std::array<bool, MAXMESHES> hasMesh;
        
        std::array<ResourceManager::KeyId,MAXMESHES> textures;
        std::array<bool, MAXMESHES> hasTexture;

        Model_Entity() noexcept{
            for(unsigned int i = 0; i < MAXMESHES;i++){
                hasMesh[i] = false;
                hasTexture[i] = false;
            }
        }

    };

    enum class lightType_enum{
        emissive,
        diffuse,
        specular
    };

    struct Light_Entity : Base_Entity{

        std::array<int,4> intensidad;
        lightType_enum type;
        //float apertura, atenAngular;
        //float atenCTE, atenlineal, atenCuadrat;

    };

    struct Camera_Entity : Base_Entity{

        SlotMap::SlotMap_Key projectionMatrix;

    };

    struct Camera_Skybox{

#ifndef __3DS__
        RandysEngine::gl_matrix viewMatrix;
        RandysEngine::gl_matrix projectionMatrix;
#else
        RandysEngine::citro_matrix viewMatrix;
        RandysEngine::citro_matrix projectionMatrix;
#endif

        Camera_Skybox(){
            projectionMatrix.perspective = true;
            projectionMatrix.fov = 45.0f;
            projectionMatrix.aspect = 400.0f/240.0f;
            projectionMatrix.near = 0.001f;
            projectionMatrix.far = 1000.0f;
        }

    };

    struct MinitreeNode{
            ////
            static constexpr unsigned int maxChildren {4};
            ////
            SlotMap::SlotMap_Key entity;
            entityType_enum type_entity{entityType_enum::none};
            ////
            SlotMap::SlotMap_Key parentNode;
            bool hasParent{false};
            ////
            SlotMap::SlotMap_Key matrixKey;
            ////
            std::array<SlotMap::SlotMap_Key,maxChildren> childrenNodes{};
            std::array<bool,maxChildren> hasChildren{};

            MinitreeNode(){
                for(unsigned int i = 0; i < maxChildren;i++){
                    hasChildren[i] = false;
                }
            }
    };

}
