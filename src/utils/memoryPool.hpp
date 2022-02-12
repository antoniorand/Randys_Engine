#pragma once

#include <cstddef>
#include <memory>
#include <cstring>
#include <assert.h>
#include <iostream>

//Based on the implementation of Misha Shalem during CppCon 2020
//https://youtu.be/l14Zkx5OXr4

namespace RandysEngine{

    namespace Pool{

        //Classes

        class Bucket{
            //Finds n free contiguous blocks in the ledger 
            //and return the first blocks index or blockcount on failure
            std::size_t find_contiguous_blocks(std::size_t n) const noexcept;
            //Marks n blocks in the ledger as "in-use" starting at 'index'
            void set_blocks_in_use(std::size_t index,std::size_t n) noexcept;
            //Marks n blocks in the ledger as "free" starting at 'index'
            void set_blocks_in_free(std::size_t index,std::size_t n) noexcept;

            //Actual memory for allocations
            std::unique_ptr<std::byte[]> m_data;
            //Reserves one bit per block to indicate wheter it is in-use
            std::unique_ptr<std::byte[]> m_ledger;

            public:
                const std::size_t blockSize;
                const std::size_t blockCount;

                Bucket(std::size_t e_block_size, std::size_t e_block_count);
                ~Bucket() = default;
                //Test if pointer belongs to bucket
                bool belongs(void * ptr) const noexcept;
                //Returns nullptr if failed
                [[nodiscard]] void* allocate(std::size_t bytes) noexcept;
                void deallocate (void* ptr,std::size_t bytes) noexcept;
        };

        struct info {
            std::size_t index{0}; //which bucket?
            std::size_t block_count{0}; //how many blocks would the allocation take from the bucket?
            std::size_t waste{0}; //How much memory would be wasted

            bool operator<(const info&other) const noexcept{
                //condition                    result if true                    result if false
                return(waste == other.waste) ? block_count < other.block_count : waste < other.waste;
            }
        };

        //Metaprogramming and configurations


        //We create a generic bucket descriptor, empty by default
        template<std::size_t id>
        struct bucket_descriptors{
            using type = std::tuple<>;
        };
        ////////////////////////////////////////////
        //We can define some bucket configurations//
        ////////////////////////////////////////////
        struct bucket_cfg16{
            static constexpr std::size_t BlockSize = 16;
            static constexpr std::size_t BlockCount = 10000;
        };

        struct bucket_cfg32{
            static constexpr std::size_t BlockSize = 32;
            static constexpr std::size_t BlockCount = 10000;
        };

        struct bucket_cfg1024{
            static constexpr std::size_t BlockSize = 1024;
            static constexpr std::size_t BlockCount = 50000;
        };
        //Define an specialization of our previous descriptor, not empty
        //We use a combination of the previous buckets we created
        template<>
        struct bucket_descriptors<1>{
            using type = std::tuple<bucket_cfg16,bucket_cfg32,bucket_cfg1024> ;
        };

        template<>
        struct bucket_descriptors<2>{
            using type = std::tuple<bucket_cfg16> ;
        };

        /////////////////////////////////////////////////////////////////////
        //The purpose of the previous descriptor is creating/////////////////
        //an array of size 3 that has these configurations in each position//
        /////////////////////////////////////////////////////////////////////

        //The type of our specialization
        template<std::size_t id>
        using bucket_descriptors_t = typename bucket_descriptors<id>::type;

        //The number of buckets we are creating in our specialization
        template<std::size_t id>
        static constexpr std::size_t bucket_count = std::tuple_size<bucket_descriptors_t<id>>::value;

        //Our pool implemented in our specialization
        template<std::size_t id>
        using pool_type = std::array<Bucket, bucket_count<id>>;

        //Black magic, apparently WIP
        template<std::size_t id, std::size_t Idx>
        struct get_size :
            std::integral_constant<std::size_t,
                std::tuple_element_t<Idx,bucket_descriptors_t<id>>::BlockSize>{};
        //Black magic, apparently WIP
        template<std::size_t id, std::size_t Idx>
        struct get_count :
            std::integral_constant<std::size_t,
                std::tuple_element_t<Idx,bucket_descriptors_t<id>>::BlockCount>{};

        //We create
        template<std::size_t id, std::size_t... Idx>
        auto & get_instance(std::index_sequence<Idx...>) noexcept{
            static pool_type<id> instance{{{get_size<id,Idx>::value,get_count<id,Idx>::value} ...}};
            return instance;
        }

        template<std::size_t id>
        auto & get_instance() noexcept{
            return get_instance<id>(std::make_index_sequence<bucket_count<id>>());
        }



    };

}