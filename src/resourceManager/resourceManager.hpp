#pragma once
#include "../slotmap/slotmap.hpp"
#include "../memoryPool/memoryPool.hpp"
#include <map>
#include <list>

//https://www.cppstories.com/2020/04/variant-virtual-polymorphism.html/

namespace RandysEngine{
    
    
    class ResourceManager{

        ///////////////////////
        //Types and constants//
        ///////////////////////
        public:
        //Id of the map for the resource, first one means type id of the resource type and second one means the id of the resource
        using mapKeyId = std::pair<std::size_t,std::size_t>;
        private:
        //Type of the value to be stored, an slotmap key
        using mapKeyValue = std::pair<std::size_t,SlotMap::SlotMap_Key>;
        //typedef SlotMap_Key mapKeyValue;
        //an allocator for mapkeys
        static constexpr unsigned int mapBlockSize = 1024;
        using mapAllocator = Pool::Static_pool_allocator<std::pair<const mapKeyId,mapKeyValue>,mapBlockSize>;

        /////////////
        //Variables//
        /////////////

        std::map<mapKeyId,
                mapKeyValue,
                std::less<mapKeyId>,
                mapAllocator> resource_map{};

        public:

            //Initialize the manager
            ResourceManager(){};
            //Destroy!!!!
            ~ResourceManager(){};

            //Get the resource after inputing the key, which is constant
            template<typename Resource_Stored>
            Resource_Stored* getResource(const mapKeyId input);

            //Store the resource in RAM memory, after that get a key that can NEVER be discarded (it has to be stored somewhere)
            template<typename Resource_Stored>
            [[nodiscard]] const mapKeyId reserveResource(Resource_Stored resource){

                //the length of the slotmap items and number of slotmaps
                static constexpr unsigned int slotmap_length = 1024;
                static constexpr unsigned int slotmaps_number = 16;
                //the next id of the next slotmap and element
                static std::size_t nextMap{0};
                static std::size_t nextResource{0};
                //slotmap allocator with a length of maximum items
                using SlotMapAlloc = RandysEngine::Pool::Static_pool_allocator<Resource_Stored,slotmap_length+1>;
                //slotmap type
                using SlotMapType = RandysEngine::SlotMap::SlotMap<Resource_Stored,SlotMapAlloc>;
                //list alloc
                using ListAlloc = RandysEngine::Pool::Static_pool_allocator<SlotMapType,slotmaps_number>;
                struct WrappedSlotMapList{
                    std::list<SlotMapType,ListAlloc> list;
                    WrappedSlotMapList(){};
                    ~WrappedSlotMapList(){
                        auto currentElements = list.size();
                        while(currentElements != 0){
                            list.pop_back();
                            currentElements--;
                        }
                    };
                };

                //list of slotmaps
                static WrappedSlotMapList SlotMapList;

                RandysEngine::ResourceManager::mapKeyId devolver;
                //Find a slotmap
                {
                    bool found = false;
                    std::size_t counter = 0;
                    for(auto& certainSlotmap : SlotMapList.list){
                        if(certainSlotmap.current_size() < slotmap_length){
                            auto key = certainSlotmap.push_back(resource);
                            //the key to be returned
                            devolver = {std::make_pair(typeid(Resource_Stored).hash_code(),nextResource++)};
                            //emplacing the resource in the map
                            resource_map.emplace(
                                devolver,
                                std::make_pair(counter,key)
                                );
                            found = true;
                            break;
                        }
                        counter++;
                    }
                    //if resource not found
                    if(!found){
                        //we create a new slotmap
                        SlotMapList.list.emplace_back(slotmap_length);
                        //we pushbach the new resource and get a key to the element
                        auto key = SlotMapList.list.back().push_back(resource);
                        //we get the key to the resource in the map
                        devolver = {std::make_pair(typeid(Resource_Stored).hash_code(),nextResource++)};
                        //we emplace the key and the value
                        resource_map.emplace(
                                devolver,
                                std::make_pair(nextMap,key)
                                );
                        //the id of the next map to be created
                        nextMap++;
                    }
                }

                return devolver;
            }

            //free the resoure with a certain key, because is not going to be used anymore or anytime soon but later
            template<typename Resource_Stored>
            bool freeResource(mapKeyId id);

            //restore the resource in RAM memory, because it was freed and we need it again
            template<typename Resource_Stored>
            bool restoreResource(Resource_Stored resource, mapKeyId Id);

            
    };

};