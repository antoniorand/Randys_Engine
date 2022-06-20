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
        
        //id of the type
        using id_of_type = std::size_t;
        //index to the specific slotmap;
        using id_of_slotmap = std::size_t;
        //key to the slotMap object
        using mapKeyValue = SlotMap::SlotMap_Key;

        //The key to be returned
        using KeyId = std::tuple<id_of_type,id_of_slotmap,mapKeyValue>;
        private:
        

        /////////////////////
        //SlotMapList class//
        /////////////////////

        //the length of the slotmap items
        template<typename Resource_Stored>
        struct slotmap_length {
            static constexpr unsigned int value = 1024;
        };

        //you can make use of template specialization of the previous class in case you need custom values

        //slotmap allocator with a length of maximum items
        template<typename Resource_Stored>
        using SlotMapAlloc = RandysEngine::Pool::Static_pool_allocator<Resource_Stored,4*(slotmap_length<Resource_Stored>::value+1)>;
            
        //slotmap type
        template<typename Resource_Stored>
        using SlotMapType = RandysEngine::SlotMap::SlotMap<Resource_Stored,SlotMapAlloc<Resource_Stored>>;

        template<typename Resource_Stored>
        struct WrappedSlotMapList{
            static constexpr unsigned int slotmaps_number = 16;
            using ListAlloc = RandysEngine::Pool::Static_pool_allocator<SlotMapType<Resource_Stored>,slotmaps_number>;
            std::list<SlotMapType<Resource_Stored>,ListAlloc> list;
            WrappedSlotMapList(){};
            ~WrappedSlotMapList(){
                auto currentElements = list.size();
                while(currentElements != 0){
                    list.pop_back();
                    currentElements--;
                }
            };
        };

        template<typename Resource_Stored>
        auto& getSlotMapList(){
            static WrappedSlotMapList<Resource_Stored> devolver;
            return (devolver.list);
        }

        /////

        public:

            //Initialize the manager
            ResourceManager(){};
            //Destroy!!!!
            ~ResourceManager(){};

            //Store the resource in RAM memory, after that get a key that can NEVER be discarded (it has to be stored somewhere)
            template<typename Resource_Stored>
            [[nodiscard]] const KeyId reserveResource(Resource_Stored resource){
                
                //the next id of the next slotmap and element
                static id_of_slotmap nextMap{0};

                //list of slotmaps
                auto& SlotMapList = getSlotMapList<Resource_Stored>();

                RandysEngine::ResourceManager::KeyId devolver;
                //Find a slotmap and try to store resource
                {
                    bool found = false;
                    id_of_slotmap counter = 0;
                    //find a free slotmap
                    for(auto& certainSlotmap : SlotMapList){
                        //if this slotmap is free, we store and return
                        if(certainSlotmap.current_size() < slotmap_length<Resource_Stored>::value){
                            auto key = certainSlotmap.push_back(resource);
                            //the key to be returned
                            devolver = {std::make_tuple(typeid(Resource_Stored).hash_code(),counter, key)};
                            found = true;
                            break;
                        }
                        counter++;
                    }
                    //if free slotmap not found
                    if(!found){
                        //we create a new slotmap
                        SlotMapList.emplace_back(slotmap_length<Resource_Stored>::value);
                        //we pushback the new resource and get a key to the element
                        auto key = SlotMapList.back().push_back(resource);
                        //we get the key to the resource in the map
                        devolver = {std::make_tuple(typeid(Resource_Stored).hash_code(),nextMap++, key)};
                    }
                }

                return devolver;
            }

            //Get the resource after inputing the key, which is constant
            template<typename Resource_Stored>
            Resource_Stored* getResource(const KeyId input){
                Resource_Stored* devolver = nullptr;
                //If this is the right resource type
                if(std::get<0>(input) == typeid(Resource_Stored).hash_code()){
                    //get the list
                    auto& SlotMapList = getSlotMapList<Resource_Stored>();
                    id_of_slotmap counter = 0;
                    //look for the slotmap it's stored
                    for(auto& certainSlotmap : SlotMapList){
                        if(counter == std::get<1>(input)){
                            //get the pointer
                            devolver = certainSlotmap.atPosition(std::get<2>(input));
                            break;
                        }
                    }
                }
                return devolver;
            }

            //free the resource with a certain key, because is not going to be used anymore
            template<typename Resource_Stored>
            bool freeResource(KeyId input){
                bool devolver = false;
                //If this is the right resource type
                if(std::get<0>(input) == typeid(Resource_Stored).hash_code()){
                    //get the list
                    auto& SlotMapList = getSlotMapList<Resource_Stored>();
                    id_of_slotmap counter = 0;
                    //look for the slotmap it's stored
                    for(auto& certainSlotmap : SlotMapList){
                        if(counter == std::get<1>(input)){
                            //DELETEEEEE
                            devolver = certainSlotmap.erase(std::get<2>(input));
                            break;
                        }
                    }
                }
                return devolver;
            }
            
    };

};