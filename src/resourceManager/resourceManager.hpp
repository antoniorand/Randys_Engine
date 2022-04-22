#pragma once
//#include "../slotmap/slotmap.hpp"
#include "../memoryPool/memoryPool.hpp"
#include <map>

//https://www.cppstories.com/2020/04/variant-virtual-polymorphism.html/

namespace RandysEngine{
    
    
    class ResourceManager{
        //static inline std::uint32_t nextID_Resource{0};

        //std::map<Pool::MapKey,void*,Pool::MapAllocator> stored_slotmaps{};

        //template<typename Resource_Stored>
        //Resource_Stored* initializeSlotMap();

        public:

            ResourceManager(){};
            ~ResourceManager(){};

            template<typename Resource_Stored>
            [[nodiscard]] std::uint32_t addResource();

    };

};