#pragma once
#include <cstdint>
#include "../memoryPool/user_defined_descriptor.hpp"
#include "../resources/model_related.hpp"
#include "../slotmap/slotmap.hpp"

namespace RandysEngine{
    
//////////////////////
///////USER CONFIG////
//////////////////////

    //Number of MAXIMUM resources and related to be stored in the RAM
    constexpr std::uint32_t NUMBER_OF_MODELS    = 500;
    constexpr std::uint32_t NUMBER_OF_MESHES    = 20*NUMBER_OF_MODELS;
    constexpr std::uint32_t NUMBER_OF_VERTICES  = 300*NUMBER_OF_MESHES;
    constexpr std::uint32_t SLOTMAP_DATA_MODELS = NUMBER_OF_MODELS + NUMBER_OF_MESHES + NUMBER_OF_VERTICES*3;
    constexpr std::uint32_t NUMBER_OF_SLOTMAPS  = SLOTMAP_DATA_MODELS;


    //Identifier of buckets
    //Just don't use descriptor 0
    constexpr std::size_t IDBUCKET_models = 100;
    constexpr std::size_t IDBUCKET_map    = 110;

    namespace Pool{

        //User defined bucket, create your own by using a personal blocksize and blockcount

        //Bucket definiton for models
        struct bucket_models{
            static constexpr std::uint32_t BlockSize = sizeof(RandysEngine::Model);
            static constexpr std::uint32_t BlockCount = NUMBER_OF_MODELS;
        };

        //Bucket definiton for meshes
        struct bucket_meshes{
            static constexpr std::uint32_t BlockSize = sizeof(Mesh);
            static constexpr std::uint32_t BlockCount = NUMBER_OF_MESHES;
        };

        //Bucket definiton for vertices
        struct bucket_vertices{
            static constexpr std::uint32_t BlockSize = sizeof(Vertex);
            static constexpr std::uint32_t BlockCount = NUMBER_OF_VERTICES;
        };

        //Bucket definiton for normals
        struct bucket_normals{
            static constexpr std::uint32_t BlockSize = sizeof(Normal);
            static constexpr std::uint32_t BlockCount = NUMBER_OF_VERTICES;
        };

        //Bucket definiton for texture coordinates
        struct bucket_textureCoords{
            static constexpr std::uint32_t BlockSize = sizeof(TextureCoords);
            static constexpr std::uint32_t BlockCount = NUMBER_OF_VERTICES;
        };

        //Bucket definiton for slotmap data
        struct bucket_slotmap_data{
            static constexpr std::uint32_t BlockSize = sizeof(std::uint32_t);
            static constexpr std::uint32_t BlockCount = SLOTMAP_DATA_MODELS*3;
        };

        //Number of Slotmaps we can store as maximum
        struct bucket_for_stored_slotmap{
            static constexpr std::uint32_t BlockSize = sizeof(SlotMap<int,Static_pool_allocator<int,IDBUCKET_models>>);
            static constexpr std::uint32_t BlockCount = NUMBER_OF_SLOTMAPS;
        };

        using MapKey = std::pair<std::uint32_t,std::uint32_t>;
        using MapKeyValue = std::pair<MapKey,void*>;
        using MapAllocator = Static_pool_allocator<MapKeyValue,IDBUCKET_map>;

        //Maximum number of slotmap pointers we can store
        struct bucket_for_ordered_map{
            static constexpr std::uint32_t BlockSize = sizeof(MapKeyValue);
            static constexpr std::uint32_t BlockCount = NUMBER_OF_SLOTMAPS;
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
            using type = std::pair<
                bucket_slotmap_data,bucket_for_ordered_map
            >;
        };
    }

}




