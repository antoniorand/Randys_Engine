#pragma once
#include <string>
#include <cstdint>
namespace RandysEngine{

    static inline std::uint32_t nextResourceId{0};

    struct Vertex{
         //This is not part of the resource interface but it will be stored as well

        //The data to be stored in a vertex
        std::uint32_t x,y,z;
        static inline const std::uint32_t typeId {nextResourceId++};
    };
    struct Normal{
         //This is not part of the resource interface but it will be stored as well

        //The data to be stored in a normal
        std::uint32_t i,j,k;
        static inline const std::uint32_t typeId {nextResourceId++};
    };
    struct TextureCoords{
         //This is not part of the resource interface but it will be stored as well

        //The data to be stored in a Texture coordinate
        std::uint32_t i,j;
        static inline const std::uint32_t typeId {nextResourceId++};
    };
    struct Mesh{
        //This is not part of the resource interface but it will be stored as well
        std::uint32_t referenceToVerteces;
        std::uint32_t referenceToNormals;
        std::uint32_t referenceToCoordinateOfTextures;
        static inline const std::uint32_t typeId {nextResourceId++};
    };

    struct Model{
        //Common characteristics of resources
        std::string resourceName;
        bool loaded {false};
        static inline const std::uint32_t typeId {nextResourceId++};
        //Specific characteristics of Model Resources        
        std::uint32_t referenceToMeshes;
    };

};