/**
 * @file memoryPool.hpp
 * @author Antonio Alberto Peguero Lopez (tony.randalph@gmail.com)
 * @title Memory Pool
 * @brief Memory Pool implementation with C++ and STL functions
 * @version 1.0
 * @date 2022-08-15
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once
#include <cstddef>
#include <memory>
#include <cstring>
#include <assert.h>
#include <iostream>
#include <vector>



//Based on the implementation of Misha Shalem during CppCon 2020
//https://youtu.be/l14Zkx5OXr4



namespace RandysEngine{

    namespace Pool{

        //Classes

        //!  Class Bucket
        /*!
        A class that has the responsability over memory stored in the Heap.
        Several objects of this class compounds a memory pool.
        */
        class Bucket{
            //! Finds n free contiguous blocks in the ledger.
            /*!
            \param n blocks to be found in a contigous manner.
            \return index number or blockcount on failure.
            \sa m_ledger(), m_data(), allocate() 
            */
            const std::uint32_t find_contiguous_blocks(std::uint32_t n) const noexcept;
            //! Sets n blocks in "used" state starting at 'index'.
            /*!
            \param index the first block in the ledger to be set
            \param n number of blocks to be set
            \sa m_ledger(), set_blocks_in_free() 
            */
            void set_blocks_in_use(std::uint32_t index,std::uint32_t n) noexcept;
            //! Sets n blocks in "free" state starting at 'index'.
            /*!
            \param index the first block in the ledger to be set
            \param n number of blocks to be set
            \sa m_ledger(), set_blocks_in_use() 
            */
            void set_blocks_in_free(std::uint32_t index,std::uint32_t n) noexcept;
            //! Current reserved memory for allocations and data storage
            /*!
                Smart pointer that points to the reserved memory in the heap for data.
                RAII design pattern is crucial in the structure of the Memory Pool and std::unique_ptr does a pretty good job here.
                Data of any type is going to be stored here
                \sa blockSize(), blockCount()
            */
            std::unique_ptr<std::byte[]> m_data;
            //! Current reserved memory for the ledger
            /*!
                Smart pointer that points to the reserved memory in the heap for the ledger.
                RAII design pattern is crucial in the structure of the Memory Pool and std::unique_ptr does a pretty good job here.
                Reserves one bit per block to indicate whether the block is in use or not.
                \sa blockSize(), blockCount()
            */
            std::unique_ptr<std::byte[]> m_ledger;

            public:
                //! Size of the block in bytes. Each block can only store one object or collection of objects.
                /*!
                    \sa blockCount()()
                */
                const std::uint32_t blockSize;
                //! Number of blocks reserved in this bucket.
                /*!
                    \sa blockCount()()
                */
                const std::uint32_t blockCount;
                //! Number of blocks reserved in this bucket.
                /*!
                    @param e_block_size
                    @param e_block_count
                    \sa blockCount()()
                */
                Bucket(std::uint32_t e_block_size, std::uint32_t e_block_count) noexcept;

                //Test if pointer belongs to bucket
                bool belongs(void * ptr) const noexcept;
                //Returns nullptr if failed
                [[nodiscard]] void* allocate(std::uint32_t bytes) noexcept;
                void deallocate (void* ptr,std::uint32_t bytes) noexcept;
        };

        //Metaprogramming and configurations


        //We create a generic bucket descriptor, empty by default
        /*template<std::size_t id>
        struct bucket_descriptors{
            using type = std::tuple<>;
        };*/

        //descriptor for types
        template<typename T,std::uint32_t MAXBLOCK_TYPE>
        struct type_bucket_descriptor{
            static constexpr std::uint32_t BlockSize = sizeof(T);
            static constexpr std::uint32_t BlockCount = MAXBLOCK_TYPE;
        };
       


        //Our pool implemented in our specialization
        template<typename T>
        //using pool_type = std::array<Bucket, bucket_count<id>>;
        using pool_type = std::vector<Bucket>;


        //We create an instance of the memory pool at the initialization of the program
        template<typename T, std::uint32_t MAXBLOCK_TYPE = 10000>
        auto & get_instance() noexcept{
            static pool_type<T> instance;
            if(instance.capacity() == 0){
                instance.reserve(10);
                instance.emplace_back(
                    type_bucket_descriptor<T,MAXBLOCK_TYPE>::BlockSize,
                    type_bucket_descriptor<T,MAXBLOCK_TYPE>::BlockCount);
            }
            return instance;
        }

        /*template<std::size_t id> //Is there a specialization ofr this id??
        constexpr bool is_defined() noexcept{
            return bucket_count <id>!=0;
        }*/

        /*template<std::size_t id>
        bool initialize() noexcept{
            (void) get_instance<id>();
            return is_defined<id>();
        }*/

        /////////////////////////
        //Memory Pool Functions//
        /////////////////////////

        /*struct info {
            std::uint32_t index{0}; //which bucket?
            std::uint32_t block_count{0}; //how many blocks would the allocation take from the bucket?
            std::uint32_t waste{0}; //How much memory would be wasted

            bool operator<(const info&other) const noexcept{
                //condition                    result if true                    result if false
                return(waste == other.waste) ? block_count < other.block_count : waste < other.waste;
            }
        };*/

        template<typename T, std::uint32_t MAXBLOCK_TYPE = 10000>
        [[nodiscard]] void * allocate(std::uint32_t bytes) noexcept{
            //Get reference to instance of pool
            auto & pool = get_instance<T,MAXBLOCK_TYPE>();
            void* devolver = nullptr;

            //get array of size of bucket count
            /*std::array<info,bucket_count<id>> deltas;
            std::uint32_t index = 0;

            for(const auto& bucket : pool){
                deltas[index].index = index;
                if(bucket.blockSize >= bytes){
                    deltas[index].waste = bucket.blockSize-bytes;
                    deltas[index].block_count = 1;
                }
                else{
                    const auto n = 1 + ((bytes - 1) /bucket.blockSize);
                    const auto storage_required = n * bucket.blockSize;
                    deltas[index].waste = storage_required - bytes;
                    deltas[index].block_count = n;
                }
                ++index;
            }
            
            std::sort(deltas.begin(),deltas.end());*/

            for(auto& bucket : pool){
                devolver = bucket.allocate(bytes);
                if(devolver)
                    break;
            }
            if(!devolver){
                pool.emplace_back(type_bucket_descriptor<T,MAXBLOCK_TYPE>::BlockSize,
                    type_bucket_descriptor<T,MAXBLOCK_TYPE>::BlockCount);
                devolver = pool.back().allocate(bytes);
            }
   
    
            return(devolver);
        }

        template<typename T, std::uint32_t MAXBLOCK_TYPE = 10000>
        void deallocate(T* ptr, std::uint32_t bytes) noexcept{
            auto & pool = get_instance<T,MAXBLOCK_TYPE>();

            for(auto & bucket  : pool){

                if(bucket.belongs(ptr)){
                    bucket.deallocate(ptr,bytes);
                    return;
                }

            }
        }

        /////////////////////////
        //Static Pool allocator//
        /////////////////////////

        //Very similar implementation to std::pmr::polymorphic_allocator
        //https://docs.w3cub.com/cpp/header/memory_resource

        template<typename T, std::uint32_t MAXBLOCK_TYPE = 10000>
        class Static_pool_allocator{
            public:

                ////////
                using value_type = T;
                ////////

                template<typename U>
                struct rebind{ using other = Static_pool_allocator<U,MAXBLOCK_TYPE>;};

                Static_pool_allocator() noexcept{}; 
                
                template<typename U>
                Static_pool_allocator(const Static_pool_allocator<U,MAXBLOCK_TYPE> & other) noexcept{}

                template<typename U, std::uint32_t MAXBLOCK_TYPE_OTHER = 10000>
                Static_pool_allocator & operator=(const Static_pool_allocator<U,MAXBLOCK_TYPE_OTHER> & other) noexcept{}

                // member functions
                [[nodiscard]] T* allocate(std::uint32_t n){
                    //std::cout << "Tipo del allocator: " << typeid(value_type).hash_code() << std::endl;
                    if(n >= MAXBLOCK_TYPE)
                        throw std::bad_alloc();
                    return static_cast<T*>(Pool::allocate<T,MAXBLOCK_TYPE>(sizeof(T)*n));
                }   
                void deallocate(T* p, std::uint32_t n){
                    if(n >= MAXBLOCK_TYPE)
                        throw std::bad_alloc();
                    Pool::deallocate<T,MAXBLOCK_TYPE>(p,n);
                }
                
                [[nodiscard]] void* allocate_bytes(std::uint32_t nbytes){
                    if(nbytes/sizeof(T) >= MAXBLOCK_TYPE)
                        throw std::bad_alloc();
                    return static_cast<T*>(Pool::allocate<T,MAXBLOCK_TYPE>(nbytes));

                }
                void deallocate_bytes(void* p, std::uint32_t nbytes){
                    if(nbytes/sizeof(T) >= MAXBLOCK_TYPE)
                        throw std::bad_alloc();
                    Pool::deallocate<T,MAXBLOCK_TYPE>(p,nbytes);
                }
                [[nodiscard]] T* allocate_object(size_t n = 1){
                    if(n >= MAXBLOCK_TYPE)
                        throw std::bad_alloc();
                    return(static_cast<T*>(this->allocate(n)));
                }
                void deallocate_object(T* p, size_t n = 1){
                    if(n >= MAXBLOCK_TYPE)
                        throw std::bad_alloc();
                    this->deallocate(p,n);
                }

        };
    };

}