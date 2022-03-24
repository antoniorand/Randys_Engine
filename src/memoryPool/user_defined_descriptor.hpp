#pragma once
#include "memoryPool.hpp"

namespace RandysEngine{

    namespace Pool{

        //User defined bucket, create your own by using a personal blocksize and blockcount
        struct bucket_1mb_8byte{
            static constexpr std::uint32_t BlockSize = 8;
            static constexpr std::uint32_t BlockCount = 32768*4;
        };
        struct bucket_1mb_16byte{
            static constexpr std::uint32_t BlockSize = 16;
            static constexpr std::uint32_t BlockCount = 32768*2;
        };
        struct bucket_1mb_32byte{
            static constexpr std::uint32_t BlockSize = 32;
            static constexpr std::uint32_t BlockCount = 32768;
        };
        struct bucket_1mb_64byte{
            static constexpr std::uint32_t BlockSize = 16;
            static constexpr std::uint32_t BlockCount = 32768/2;
        };

        //user defined descripto, create your own memory pool by joining several buckets
        //just don't use descriptor 0
        template<>
        struct bucket_descriptors<10>{
            using type = std::tuple<
                bucket_1mb_8byte,bucket_1mb_8byte,bucket_1mb_8byte,
                bucket_1mb_16byte,bucket_1mb_16byte,bucket_1mb_16byte,
                bucket_1mb_32byte,bucket_1mb_32byte,bucket_1mb_32byte,
                bucket_1mb_64byte,bucket_1mb_64byte,bucket_1mb_64byte
            >;
        };

    }

}

