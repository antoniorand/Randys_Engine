#pragma once
#include "resourceManager.hpp"
#include "../APIs/wrappers/gl_wrapper.hpp"

namespace RandysEngine{

    enum class entityType_enum{
        model,
        light,
        camera
    };

    struct Model_Entity{

        ResourceManager::KeyId mesh_resource;

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
            using VectorChildren = std::array<
                std::tuple<ResourceManager::KeyId,bool>,maxChildren>;

            SlotMap::SlotMap_Key entity;
            entityType_enum type_entity;

            ResourceManager::KeyId parentNode;
            VectorChildren childNodes;

    };

}
