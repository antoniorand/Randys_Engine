#pragma once
#include <cstdint>
#include "../memoryPool/user_defined_descriptor.hpp"
#include "../resources/model_related.hpp"

namespace RandysEngine{
    
//////////////////////
///////USER CONFIG////
//////////////////////

    //Number of MAXIMUM resources and related to be stored in the RAM
    constexpr std::uint32_t NUMBER_OF_MODELS    = 500;
    constexpr std::uint32_t NUMBER_OF_MESHES    = 20*NUMBER_OF_MODELS;
    constexpr std::uint32_t NUMBER_OF_VERTICES  = 300*NUMBER_OF_MESHES;
    constexpr std::uint32_t SLOTMAP_DATA_MODELS = NUMBER_OF_MODELS + NUMBER_OF_MESHES + NUMBER_OF_VERTICES*3;


    //Identifier of buckets
    //Just don't use descriptor 0
    constexpr std::size_t IDBUCKET_models = 100;
    constexpr std::size_t IDBUCKET_map    = 110;

    namespace Pool{

        //User defined bucket, create your own by using a personal blocksize and blockcount
        struct bucket_models{
            static constexpr std::uint32_t BlockSize = sizeof(RandysEngine::Model);
            static constexpr std::uint32_t BlockCount = NUMBER_OF_MODELS;
        };
        struct bucket_meshes{
            static constexpr std::uint32_t BlockSize = sizeof(Mesh);
            static constexpr std::uint32_t BlockCount = NUMBER_OF_MESHES;
        };
        struct bucket_vertices{
            static constexpr std::uint32_t BlockSize = sizeof(Vertex);
            static constexpr std::uint32_t BlockCount = NUMBER_OF_VERTICES;
        };
        struct bucket_normals{
            static constexpr std::uint32_t BlockSize = sizeof(Normal);
            static constexpr std::uint32_t BlockCount = NUMBER_OF_VERTICES;
        };
        struct bucket_textureCoords{
            static constexpr std::uint32_t BlockSize = sizeof(TextureCoords);
            static constexpr std::uint32_t BlockCount = NUMBER_OF_VERTICES;
        };
        struct bucket_slotmap_data{
            static constexpr std::uint32_t BlockSize = sizeof(std::uint32_t);
            static constexpr std::uint32_t BlockCount = SLOTMAP_DATA_MODELS;
        };

        struct bucket_for_ordered_map{
            static constexpr std::uint32_t BlockSize = 72;
            static constexpr std::uint32_t BlockCount = 10;
        };

        //user defined descriptor, create your own memory pool by joining several buckets
        //just don't use descriptor 0
        template<>
        struct bucket_descriptors<IDBUCKET_models>{
            using type = std::tuple<
                bucket_models, bucket_meshes, bucket_vertices, bucket_normals,bucket_textureCoords,
                bucket_slotmap_data
            >;
        };

        template<>
        struct bucket_descriptors<IDBUCKET_map>{
            using type = std::tuple<bucket_for_ordered_map,bucket_for_ordered_map>;
        };
    }

}




