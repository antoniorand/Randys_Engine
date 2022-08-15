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

        class Bucket{
            //Finds n free contiguous blocks in the ledger 
            //and return the first blocks index or blockcount on failure
            const std::uint32_t find_contiguous_blocks(std::uint32_t n) const noexcept;
            //Marks n blocks in the ledger as "in-use" starting at 'index'
            void set_blocks_in_use(std::uint32_t index,std::uint32_t n) noexcept;
            //Marks n blocks in the ledger as "free" starting at 'index'
            void set_blocks_in_free(std::uint32_t index,std::uint32_t n) noexcept;

            //Actual memory for allocations
            std::unique_ptr<std::byte[]> m_data;
            //Reserves one bit per block to indicate wheter it is in-use
            std::unique_ptr<std::byte[]> m_ledger;

            public:
                const std::uint32_t blockSize;
                const std::uint32_t blockCount;

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
                template<class... CtorArgs> T* new_object(CtorArgs&&... ctor_args){
                    T* devolver = static_cast<T*>(
                        this->allocate_object()
                    );
                    return(construct(devolver,ctor_args...));
                }
                void delete_object(T* p){
                    destroy(p);
                    this->deallocate_object(p);
                }

                template<class... Args>
                void construct(T* p, Args&&... args){
                    new ((void*)p) T (args...);
                }
                
                void destroy(T* p){
                    (p)->~T();
                }

        };
    };

}