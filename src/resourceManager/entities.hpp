#pragma once
#include "resourceManager.hpp"

namespace RandysEngine{

    enum class entityType_enum{
        model,
        light,
        camera
    };

    struct Model_Entity{

        static constexpr unsigned int MAXMESHES {4};

        std::array<ResourceManager::KeyId,MAXMESHES> meshes;
        std::array<bool, MAXMESHES> hasMesh;
        
        std::array<ResourceManager::KeyId,MAXMESHES> textures;
        std::array<bool, MAXMESHES> hasTexture;

        Model_Entity(){
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

    struct Light_Entity{

        std::array<int,4> intensidad;
        lightType_enum type;
        //float apertura, atenAngular;
        //float atenCTE, atenlineal, atenCuadrat;

    };

    struct Camera_Entity{

        float left, right, lower, upper, close, far;

    };

    struct MinitreeNode{
            static constexpr unsigned int maxChildren {4};

            SlotMap::SlotMap_Key entity;
            entityType_enum type_entity;

            SlotMap::SlotMap_Key parentNode;
            bool hasParent{false};

            ////
            std::array<SlotMap::SlotMap_Key,maxChildren> childrenNodes{};
            std::array<bool,maxChildren> hasChildren{};
    };

}
