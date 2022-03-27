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

        template<typename Resource_Stored>
        Resource_Stored* initializeSlotMap();

        public:

            ResourceManager(){};
            ~ResourceManager(){};

            template<typename Resource_Stored>
            [[nodiscard]] std::uint32_t addResource(){
                //The resource id to be stored
                std::uint32_t devolver = nextID_Resource++;
                /*Resource_Stored* recurso = initializeSlotMap<Resource_Stored>();
                //std::cout << "Id del recurso: " << devolver << std::endl;
                //std::cout << "Id del tipo de recurso: " << Resource_Stored::typeId << std::endl;
                void* toBeStored = static_cast<void*>(recurso);
                //stored_slotmaps.emplace(std::make_pair<Resource_Stored::typeId,devolver>);*/

                return devolver;
            }

    };

};