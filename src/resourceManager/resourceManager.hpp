#pragma once
#include "user_defined_sizes.hpp"
#include "../slotmap/slotmap.hpp"
#include "../memoryPool/memoryPool.hpp"
#include <map>

//https://www.cppstories.com/2020/04/variant-virtual-polymorphism.html/

namespace RandysEngine{
    
    
    class ResourceManager{
        static inline std::uint32_t nextID_Resource{0};

        std::map<Pool::MapKey,void*,Pool::MapAllocator> stored_slotmaps{};

        public:

            ResourceManager(){};
            ~ResourceManager(){};

            template<typename Resource_Stored>
            std::uint32_t addResource(){

                std::uint32_t devolver = nextID_Resource++;
                std::cout << "Id del recurso: " << devolver << std::endl;
                std::cout << "Id del tipo de recurso: " << Resource_Stored::typeId << std::endl;

                return devolver;
            }

    };

};