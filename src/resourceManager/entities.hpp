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

        float left, right, lower, upper, close, far;

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
