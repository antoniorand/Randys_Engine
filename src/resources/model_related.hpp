#pragma once
#include <string>
#include <cstdint>
namespace RandysEngine{

    struct Vertex{
        //The data to be stored in a vertex
        std::uint32_t x,y,z;

    };
    struct Normal{
        //The data to be stored in a normal
        std::uint32_t i,j,k;
    };
    struct TextureCoords{
        //The data to be stored in a Texture coordinate
        std::uint32_t i,j;
    };
    struct Mesh{
        //This is not part of the resource interface but it will be stored as well
        std::uint32_t referenceToVerteces;
        std::uint32_t referenceToNormals;
        std::uint32_t referenceToCoordinateOfTextures;
    };

    struct Model{
        //Common characteristics of resources
        std::string resourceName;
        bool loaded {false};
        //Specific characteristics of Model Resources        
        std::uint32_t referenceToMeshes;
        
    };

};