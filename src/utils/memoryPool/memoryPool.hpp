#pragma once
#include <cstddef>
#include <memory>
#include <cstring>
#include <assert.h>
#include <iostream>
#ifdef __clang__
    #include <experimental/memory_resource>
#else
    #include <memory_resource>
#endif



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

                //Test if pointer belongs to bucket
                bool belongs(void * ptr) const noexcept;
                //Returns nullptr if failed
                [[nodiscard]] void* allocate(std::size_t bytes) noexcept;
                void deallocate (void* ptr,std::size_t bytes) noexcept;
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
        /////////////////////////////////////////////////////
        //Example on how to define your own specializations//
        /////////////////////////////////////////////////////
        /*struct bucket_cfg16{
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
        };*/

        //In a L1 cache of size of 256kbs
        //You can store up to 15 blocks of the following
        //blocksize and blockcount
        struct bucket_L1_256kbs{
            static constexpr std::size_t BlockSize = 16;
            //1 + ((blockCount-1)/8); 128 ledger size
            static constexpr std::size_t BlockCount = 1024;
        };

        //Define an specialization of our previous descriptor, not empty
        //We use a combination of the previous buckets we created
        /*template<>
        struct bucket_descriptors<1>{
            using type = std::tuple<bucket_cfg16,bucket_cfg32,bucket_cfg1024> ;
        };

        template<>
        struct bucket_descriptors<2>{
            using type = std::tuple<bucket_cfg16,bucket_cfg16,bucket_cfg16> ;
        };*/

        template<>
        struct bucket_descriptors<0>{
            using type = std::tuple<
                bucket_L1_256kbs,bucket_L1_256kbs,bucket_L1_256kbs,
                bucket_L1_256kbs,bucket_L1_256kbs,bucket_L1_256kbs,
                bucket_L1_256kbs,bucket_L1_256kbs,bucket_L1_256kbs,
                bucket_L1_256kbs,bucket_L1_256kbs,bucket_L1_256kbs,
                bucket_L1_256kbs,bucket_L1_256kbs,bucket_L1_256kbs
            >;
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

        template<std::size_t id> //Is there a specialization ofr this id??
        constexpr bool is_defined() noexcept{
            return bucket_count <id>!=0;
        }

        template<std::size_t id>
        bool initialize() noexcept{
            (void) get_instance<id>();
            return is_defined<id>();
        }

        /////////////////////////
        //Memory Pool Functions//
        /////////////////////////

        struct info {
            std::size_t index{0}; //which bucket?
            std::size_t block_count{0}; //how many blocks would the allocation take from the bucket?
            std::size_t waste{0}; //How much memory would be wasted

            bool operator<(const info&other) const noexcept{
                //condition                    result if true                    result if false
                return(waste == other.waste) ? block_count < other.block_count : waste < other.waste;
            }
        };

        template<std::size_t id>
        [[nodiscard]] void * allocate(std::size_t bytes){

            auto & pool = get_instance<id>();

            std::array<info,bucket_count<id>> deltas;
            std::size_t index = 0;

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

                std::sort(deltas.begin(),deltas.end());

                for(const auto & d : deltas){
                    if(auto ptr = pool[d.index].allocate(bytes);ptr != nullptr)
                        return ptr;
                }

                throw std::bad_alloc();
            }
            return(nullptr);
        }

        template<std::size_t id>
        void deallocate(void* ptr, std::size_t bytes) noexcept{
            auto & pool = get_instance<id>();

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

        template<typename T = std::uint8_t, std::size_t id =0>
        class Static_pool_allocator{
            public:

                ////////
                using value_type = T;
                ////////

                template<typename U>
                struct rebind{ using other = Static_pool_allocator<U,id>;};
#ifdef __clang__
                Static_pool_allocator() noexcept : m_upstream_resource{std::experimental::pmr::get_default_resource()}{}
                Static_pool_allocator(std::experimental::pmr::memory_resource * res) noexcept : m_upstream_resource{res}{}
#else
                Static_pool_allocator() noexcept : m_upstream_resource{std::pmr::get_default_resource()}{}
                Static_pool_allocator(std::pmr::memory_resource * res) noexcept : m_upstream_resource{res}{}
#endif              
                
                template<typename U>
                Static_pool_allocator(const Static_pool_allocator<U,id> & other) noexcept
                    : m_upstream_resource{other.upstream_resource()}{}

                template<typename U>
                Static_pool_allocator & operator=(const Static_pool_allocator<U,id> & other) noexcept{
                    m_upstream_resource = other.upstream_resource();
                }

                template<typename U>
                constexpr bool operator== (const Static_pool_allocator<U,id>& other) noexcept{
                    return (this->m_upstream_resource == other.m_upstream_resource);
                }

                template<typename U>
                constexpr bool operator!= (const Static_pool_allocator<U,id>& other) noexcept{
                    return (this->m_upstream_resource != other.m_upstream_resource);
                }

                // member functions
                [[nodiscard]] T* allocate(size_t n){
                    if constexpr (Pool::is_defined<id>()){
                        return static_cast<T*>(Pool::allocate<id>(sizeof(T)*n));
                    }
                    else if(m_upstream_resource != nullptr){
                        return static_cast<T*>(m_upstream_resource->allocate(sizeof(T)*n,alignof(T)));
                    }  else{
                        throw std::bad_alloc();
                    }
                }   
                void deallocate(T* p, size_t n){
                    if constexpr (Pool::is_defined<id>()){
                        Pool::deallocate<id>(p,n);
                    }
                    else if(m_upstream_resource != nullptr){
                        m_upstream_resource->deallocate(p,n,alignof(T));
                    }
                }
                
                [[nodiscard]] void* allocate_bytes(size_t nbytes, size_t alignment = alignof(max_align_t)){
                    if constexpr (Pool::is_defined<id>()){
                        return static_cast<T*>(Pool::allocate<id>(nbytes));
                    }
                    else if(m_upstream_resource != nullptr){
                        return static_cast<T*>(m_upstream_resource->allocate(nbytes,alignment));
                    }  else{
                        throw std::bad_alloc();
                    }
                }
                void deallocate_bytes(void* p, size_t nbytes, size_t alignment = alignof(max_align_t)){
                    if constexpr (Pool::is_defined<id>()){
                        Pool::deallocate<id>(p,nbytes);
                    }
                    else if(m_upstream_resource != nullptr){
                        m_upstream_resource->deallocate(p,nbytes,alignment);
                    }
                }
                [[nodiscard]] T* allocate_object(size_t n = 1){
                    return(static_cast<T*>(this->allocate(n)));
                }
                void deallocate_object(T* p, size_t n = 1){
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

                /*polymorphic_allocator select_on_container_copy_construction() const;*/

                static bool initialize_memory_pool() noexcept 
                    {return Pool::initialize<id>();};
#ifdef __clang__
                std::experimental::pmr::memory_resource * upstream_resource() const noexcept{
                    return m_upstream_resource;
                }
#else
                std::pmr::memory_resource * upstream_resource() const noexcept{
                    return m_upstream_resource;
                }
#endif 

            private:
#ifdef __clang__
                std::experimental::pmr::memory_resource * m_upstream_resource;
#else
                std::pmr::memory_resource * m_upstream_resource;
#endif 

        };
    };

}