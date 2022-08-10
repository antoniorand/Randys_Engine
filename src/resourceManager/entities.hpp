#pragma once
#include "resourceManager.hpp"
#include "../APIs/wrappers/gl_wrapper.hpp"

namespace RandysEngine{

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
            
            using VectorChildren = std::vector<
                ResourceManager::KeyId,
                RandysEngine::Pool::Static_pool_allocator<ResourceManager::KeyId,1024*4>
                >;

            static const unsigned int maxChildren {4};
            
            std::variant<
                RandysEngine::Light_Entity,
                RandysEngine::Model_Entity,
                RandysEngine::Camera_Entity
                > element;

            ResourceManager::KeyId parentNode;
            VectorChildren childNodes;

            MinitreeNode(){
                childNodes.reserve(maxChildren);
            }
    };

}
