/**
 * @file memoryPool.hpp
 * @author Antonio Alberto Peguero Lopez (tony.randalph@gmail.com)
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

        /*! \defgroup bucket_class Bucket class
        *  Bucket class documentation
        *  @{
        */

        //!  Class Bucket
        /*!
        A class that has the responsability over memory stored in the Heap.
        Several objects of this class compounds a memory pool.
        */
        class Bucket{
            //! Finds n free contiguous blocks in the ledger.
            /*!
            @param n blocks to be found in a contigous manner.
            @return index number or blockcount on failure.
            @sa m_ledger, m_data, allocate() 
            */
            const std::uint32_t find_contiguous_blocks(std::uint32_t n) const noexcept;
            //! Sets n blocks in "used" state starting at 'index'.
            /*!
            @param index the first block in the ledger to be set
            @param n number of blocks to be set
            @sa m_ledger, set_blocks_in_free() 
            */
            void set_blocks_in_use(std::uint32_t index,std::uint32_t n) noexcept;
            //! Sets n blocks in "free" state starting at 'index'.
            /*!
            @param index the first block in the ledger to be set
            @param n number of blocks to be set
            @sa m_ledger, set_blocks_in_use() 
            */
            void set_blocks_in_free(std::uint32_t index,std::uint32_t n) noexcept;
            //! Current reserved memory for allocations and data storage
            /*!
                Smart pointer that points to the reserved memory in the heap for data.
                RAII design pattern is crucial in the structure of the Memory Pool and std::unique_ptr does a pretty good job here.
                Data of any type is going to be stored here
                @sa blockSize, blockCount, m_ledger
            */
            std::unique_ptr<std::byte[]> m_data;
            //! Current reserved memory for the ledger
            /*!
                Smart pointer that points to the reserved memory in the heap for the ledger.
                RAII design pattern is crucial in the structure of the Memory Pool and std::unique_ptr does a pretty good job here.
                Reserves one bit per block to indicate whether the block is in use or not.
                @sa blockSize, blockCount, m_data
            */
            std::unique_ptr<std::byte[]> m_ledger;

            public:
                //! Size of the block in bytes. Each block can only store one object or collection of objects.
                /*!
                    @sa blockCount, m_data, m_ledger
                */
                const std::uint32_t blockSize;
                //! Number of blocks reserved in this bucket.
                /*!
                    @sa blockSize
                */
                const std::uint32_t blockCount;
                //! Constructor of class.
                /*!
                    It will reserve 'n' bytes of memory in the heap in which 'n'
                    is e_block_size multiplied e_block_count times.
                    @param e_block_size size of the block to be reserved in the heap.
                    @param e_block_count number of the blocks to be reserved in the heap.
                    @sa blockCount
                    @sa blockSize
                */
                Bucket(std::uint32_t e_block_size, std::uint32_t e_block_count) noexcept;
                //! Pointer tester
                /*!
                    It will check if the pointer belongs to an area of memory reserved in this bucket.
                    @param ptr pointer to be checked.
                    @return false if the poiner doesn't belong to the bucker, true otherwise.
                    @sa blockCount
                    @sa blockSize
                */
                bool belongs(void * ptr) const noexcept;
                //! Allocate memory in the bucket
                /*!
                    Tries to reserve memory bucket, unless the current bucket can't.
                    @param bytes number of bytes specified by the user. The bucket has to reserve them from its blocks.
                    @return nullptr if you can't reserve the specified bytes, otherwise the pointer to the allocated memory
                    @sa blockCount
                    @sa blockSize
                */
                [[nodiscard]] void* allocate(std::uint32_t bytes) noexcept;
                //! Deallocate memory in the bucket
                /*!
                    Frees memory inside the bucket in a contiguous manner
                    @param ptr pointer with the first byte to be freed.
                    @param bytes number of bytes to be freed. The bucket also clears their position in the ledger.
                    @sa blockCount
                    @sa blockSize
                */
                void deallocate (void* ptr,std::uint32_t bytes) noexcept;
        };

        /*! @} */

        /*! \defgroup metaprogramming_memory_pool Metaprogramming and configurations
        *  Configuration using templates so you can use any type of bucket that you need
        *  @{
        */

        //! Descriptor of the bucket, which depends on the type and size
        /*!
        A class that generates through it's template parameter during compilation.
        It defines a bucket of sizeof(T) bytes in which T is an object and a max number of blocks.
        @tparam T type of the class or data struct to be stored.
        @tparam MAXBLOCK_TYPE number of blocks in each memory poolç
        @sa pool_type,get_instance()
        */
        //descriptor for types
        template<typename T,std::uint32_t MAXBLOCK_TYPE>
        struct type_bucket_descriptor{
            static constexpr std::uint32_t BlockSize = sizeof(T);
            static constexpr std::uint32_t BlockCount = MAXBLOCK_TYPE;
        };
       

        //! Memory pool definition
        /*!
        A memory pool is a collection of buckets of a certain type
        @tparam T type of the class or data struct to be stored.
        @sa type_bucket_descriptor,get_instance()
        */
        template<typename T>
        using pool_type = std::vector<Bucket>;


        //! Memory pool instance creation
        /*!
            Creates an instance of the memory pool when needed the first time.
            @tparam T type of the class or data struct to be stored.
            @tparam MAXBLOCK_TYPE number of blocks in each memory pool, 10.000 by default.
            @return an instance of the memory pool, which is static (only needs to be created the first time).
            @sa type_bucket_descriptor,pool_type
        */
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

        /*! @} */

        /////////////////////////
        //Memory Pool Functions//
        /////////////////////////

        /*! \defgroup functions_memory_pool Memory Pool functions
        *  General functions and logic of the memory pool
        *  @{
        */


        /*struct info {
            std::uint32_t index{0}; //which bucket?
            std::uint32_t block_count{0}; //how many blocks would the allocation take from the bucket?
            std::uint32_t waste{0}; //How much memory would be wasted

            bool operator<(const info&other) const noexcept{
                //condition                    result if true                    result if false
                return(waste == other.waste) ? block_count < other.block_count : waste < other.waste;
            }
        };*/

        //! Memory pool allocation
        /*!
        Reserves data in the specified memory pool.
        @tparam T type of the class or data struct to be stored.
        @tparam MAXBLOCK_TYPE number of blocks in each memory pool, 10.000 by default.
        @param bytes number of bytes to be reserved in the memory pool.
        @return pointer to the reserved memory that can be used. Nullptr if failed.
        @sa get_instance(), deallocate()
        */
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
        //! Memory pool deallocation
        /*!
        Frees data in the specified memory pool.
        @tparam T type of the class or data struct to be stored.
        @tparam MAXBLOCK_TYPE number of blocks in each memory pool, 10.000 by default.
        @param ptr pointer in which the data to be freed is located.
        @param bytes number of bytes to be reserved in the memory pool.
        @sa deallocate()
        */
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

        /*! @} */

        /////////////////////////
        //Static Pool allocator//
        /////////////////////////

        /*! \defgroup static_allocator Static Pool Allocator
        *  An allocator that uses a memory pool for general purposes
        *  @{
        */

        //Very similar implementation to std::pmr::polymorphic_allocator
        //https://docs.w3cub.com/cpp/header/memory_resource

        //!  Class Static Pool Allocator
        /*!
        A class of general purposes that uses the std::allocator interface and can be used in stl data structures.
        */
        template<typename T, std::uint32_t MAXBLOCK_TYPE = 10000>
        class Static_pool_allocator{
            public:

                //! Value Type
                /*!
                Name of the type to be reserved in the allocator
                */
                using value_type = T;

                //! rebind function
                /*!
                Creates a definition of the memory pool using another type
                @tparam U type of the class or data struct to be stored.
                */
                template<typename U>
                struct rebind{ using other = Static_pool_allocator<U,MAXBLOCK_TYPE>;};

                //! Constructor of the class
                /*!
                Initializes the allocator.
                */
                Static_pool_allocator() noexcept{}; 
                //! Copy constructor of the class
                /*!
                Initializes the allocator using another one as reference.
                @tparam U type of the class or data struct to be stored.
                */
                template<typename U>
                Static_pool_allocator(const Static_pool_allocator<U,MAXBLOCK_TYPE> & other) noexcept{}

                // member functions

                //! Allocates memory using the memory pool.
                /*!
                @param n number of objects to be reserved.
                @return pointer to the objects or objects.
                */
                [[nodiscard]] T* allocate(std::uint32_t n){
                    //std::cout << "Tipo del allocator: " << typeid(value_type).hash_code() << std::endl;
                    if(n >= MAXBLOCK_TYPE)
                        throw std::bad_alloc();
                    return static_cast<T*>(Pool::allocate<T,MAXBLOCK_TYPE>(sizeof(T)*n));
                }   
                //! Deallocates memory using the memory pool.
                /*!
                @param pointer pointer to the objects to be freed.
                @param n number of objects to be freed.
                */
                void deallocate(T* p, std::uint32_t n){
                    if(n >= MAXBLOCK_TYPE)
                        throw std::bad_alloc();
                    Pool::deallocate<T,MAXBLOCK_TYPE>(p,n);
                }
                //! Allocates bytes from the memory pool.
                /*!
                @param nbytes number of bytes to be reserved.
                @return pointer to the reserved memory.
                */
                [[nodiscard]] void* allocate_bytes(std::uint32_t nbytes){
                    if(nbytes/sizeof(T) >= MAXBLOCK_TYPE)
                        throw std::bad_alloc();
                    return static_cast<T*>(Pool::allocate<T,MAXBLOCK_TYPE>(nbytes));

                }
                //! Deallocates bytes from the memory pool.
                /*!
                @param p pointer to the data to be freed.
                @param nbytes number of bytes to be freed.
                */
                void deallocate_bytes(void* p, std::uint32_t nbytes){
                    if(nbytes/sizeof(T) >= MAXBLOCK_TYPE)
                        throw std::bad_alloc();
                    Pool::deallocate<T,MAXBLOCK_TYPE>(p,nbytes);
                }
                //! Allocates objects from the memory pool.
                /*!
                @param n number of object to be reserved.
                @return pointer to the reserved memory.
                */
                [[nodiscard]] T* allocate_object(size_t n = 1){
                    if(n >= MAXBLOCK_TYPE)
                        throw std::bad_alloc();
                    return(static_cast<T*>(this->allocate(n)));
                }
                //! Deallocates objects from the memory pool.
                /*!
                @param p pointer to the data to be freed.
                @param n number of bytes to be freed.
                */
                void deallocate_object(T* p, size_t n = 1){
                    if(n >= MAXBLOCK_TYPE)
                        throw std::bad_alloc();
                    this->deallocate(p,n);
                }

        };

        /*! @} */
    };

}