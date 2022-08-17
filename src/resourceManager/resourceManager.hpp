#pragma once
#include "../slotmap/slotmap.hpp"
#include "../memoryPool/memoryPool.hpp"
#include <map>
#include <deque>
#include <typeinfo>

//https://www.cppstories.com/2020/04/variant-virtual-polymorphism.html/

namespace RandysEngine{
    
    
    
    class ResourceManager{

        public:

        ///////////////////////
        //Types and constants//
        ///////////////////////
            
        //id of the type
        using idType = std::size_t;
        //index to the specific slotmap;
        using indexSlotmap = std::size_t;

        //The key to be returned
        struct KeyId{
            idType type_id;
            indexSlotmap index;
            SlotMap::SlotMap_Key key;
        };

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
        struct SlotMapListData{
            //NOTE: if you specialize this class, NEVER EVER use slotmaps_number under value 16 
            //because that's the minimun size for std::DEQUE
            static constexpr unsigned int slotmaps_number = 16;
            //the next id of the next slotmap and element
            static inline indexSlotmap nextMap{0};
        };

        template<typename Resource_Stored>
        using ListAlloc = RandysEngine::Pool::Static_pool_allocator<
            SlotMapType<Resource_Stored>, 
            SlotMapListData<Resource_Stored>::slotmaps_number+2
        >;

        struct baseList{

            baseList(){};

            virtual ~baseList(){};

        };

        //slotmap list
        template<typename Resource_Stored>
        struct SlotMapList : baseList {
            std::deque<SlotMapType<Resource_Stored>,ListAlloc<Resource_Stored>> list;
        
            ~SlotMapList(){};
        };


        std::map<std::size_t,
            baseList*> listMap;

        template<typename Resource_Stored>
        SlotMapList<Resource_Stored>& getSlotMapList(){
            
            SlotMapList<Resource_Stored>* devolver = nullptr;

            auto it = listMap.find(typeid(SlotMapList<Resource_Stored>).hash_code());

            if(it != listMap.end()){
                devolver = static_cast<SlotMapList<Resource_Stored>*>(it->second);
            }
            else{
                const std::size_t typeNumber = typeid(SlotMapList<Resource_Stored>).hash_code();
                std::unique_ptr<baseList> listPtr = std::make_unique<SlotMapList<Resource_Stored>>();
                listMap[typeNumber] = listPtr.release();
                devolver = static_cast<SlotMapList<Resource_Stored>*>(listMap[typeNumber]);
            }

            return (*devolver);
        }

        /////

        public:

            //Initialize the manager
            ResourceManager(){};
            //Destroy!!!!
            ~ResourceManager(){
                for(auto& pair : listMap){
                    std::unique_ptr<baseList>(pair.second);
                }
            };

            //Store the resource in RAM memory, after that get a key that can NEVER be discarded (it has to be stored somewhere)
            template<typename Resource_Stored>
            [[nodiscard]] const KeyId reserveResource(Resource_Stored&& resource){
                
                //list of slotmaps
                auto& SlotMapList = getSlotMapList<Resource_Stored>().list;

                RandysEngine::ResourceManager::KeyId devolver;
                //Find a slotmap and try to store resource
                {
                    bool found = false;
                    indexSlotmap counter = 0;
                    //find a free slotmap
                    for(auto& certainSlotmap : SlotMapList){
                        //if this slotmap is free, we store and return
                        if(certainSlotmap.current_size() < slotmap_length<Resource_Stored>::value){
                            auto key = certainSlotmap.push_back(std::forward<Resource_Stored>(resource));
                            //the key to be returned
                            devolver = {typeid(Resource_Stored).hash_code(),counter, key};
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
                        auto key = SlotMapList.back().push_back(std::forward<Resource_Stored>(resource));
                        //we get the key to the resource in the map
                        devolver = {typeid(Resource_Stored).hash_code(),SlotMapListData<Resource_Stored>::nextMap++, key};
                    }
                }

                return devolver;
            }

            template<typename Resource_Stored>
            [[nodiscard]] const KeyId reserveResource(Resource_Stored& resource){
                return(this->reserveResource(Resource_Stored{resource}));
            }

            template<typename Resource_Stored,class... Args>
            [[nodiscard]] const KeyId createResource(Args... input){
                //list of slotmaps
                auto& SlotMapList = getSlotMapList<Resource_Stored>().list;

                RandysEngine::ResourceManager::KeyId devolver;
                //Find a slotmap and try to store resource
                {
                    bool found = false;
                    indexSlotmap counter = 0;
                    //find a free slotmap
                    for(auto& certainSlotmap : SlotMapList){
                        //if this slotmap is free, we store and return
                        if(certainSlotmap.current_size() < slotmap_length<Resource_Stored>::value){
                            auto key = certainSlotmap.emplace_back(input...);
                            //the key to be returned
                            devolver = {typeid(Resource_Stored).hash_code(),counter, key};
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
                        auto key = SlotMapList.back().emplace_back(input...);
                        //we get the key to the resource in the map
                        devolver = {typeid(Resource_Stored).hash_code(),SlotMapListData<Resource_Stored>::nextMap++, key};
                    }
                }
                return devolver;
            }

            //Get the resource after inputing the key, which is constant
            template<typename Resource_Stored>
            Resource_Stored* getResource(const KeyId input){
                Resource_Stored* devolver = nullptr;
                //If this is the right resource type
                if(input.type_id == typeid(Resource_Stored).hash_code()){
                    //get the list
                    auto& SlotMapList = getSlotMapList<Resource_Stored>().list;
                    indexSlotmap counter = 0;
                    //look for the slotmap it's stored
                    for(auto& certainSlotmap : SlotMapList){
                        if(counter == input.index){
                            //get the pointer
                            devolver = certainSlotmap.atPosition(input.key);
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
                if(input.type_id == typeid(Resource_Stored).hash_code()){
                    //get the list
                    auto& SlotMapList = getSlotMapList<Resource_Stored>();
                    indexSlotmap counter = 0;
                    //look for the slotmap it's stored
                    for(auto& certainSlotmap : SlotMapList){
                        if(counter == input.index){
                            //DELETEEEEE
                            devolver = certainSlotmap.erase(input.key);
                            break;
                        }
                    }
                }
                return devolver;
            }
            
    };

};