#pragma once
#include "../slotmap/slotmap.hpp"
#include "../memoryPool/memoryPool.hpp"
#include <map>
#include <deque>
#include <typeinfo>

//https://www.cppstories.com/2020/04/variant-virtual-polymorphism.html/

namespace RandysEngine{
    
    /*! \defgroup res_man Resource Manager
    *  Resource Manager class documentation
    *  @{
    */

    //!  Class ResourceManager
    /*!
        This class reserves data using a collection of slotmaps, which you don't need to specify any of the size
        during run-time. So all of the data reservation is invisible to the external programmer, accomplishing
        the specification of the 'façade' design pattern.

        This resource manager implementation is a little bit complex, so this documentation
        will tell in detail how it works, so the people that want to optimize it will get an idea.
    */
    
    class ResourceManager{

        public:

        ///////////////////////
        //Types and constants//
        ///////////////////////
            
        //! Id of the data type that is going to be stored inside the resource manager.
        /*!
            @sa KeyId
            @sa indexSlotmap
            @sa reserveResource
            @sa createResource
            @sa getResource
            @sa freeResource
        */
        using idType = std::size_t;
        //! Index of the specific slotmap that the data is currently being stored.
        /*!
            @sa KeyId
            @sa idType
            @sa reserveResource
            @sa createResource
            @sa getResource
            @sa freeResource
        */
        using indexSlotmap = std::size_t;

        //! Key that references the data inside of the Resource Manager
        /*!
            Each time you store data inside the Resource Manager it will return a KeyId.
            This KeyId SHOULD ALWAYS BE STORED IN A VARIABLE, hence the [[nodiscard]] attribute.
            If you lose, somehow, the KeyId, the data will be unable to be freed until the end of the
            execution of the program.
            I recommend using std::map or std::unordered_map for storing these.
            @sa idType
            @sa indexSlotmap
            @sa reserveResource
            @sa createResource
            @sa getResource
            @sa freeResource
        */
        struct KeyId{
            //! Id of the type
            /*!
                In c++ exists an operator called typeid() which returns data called type_info.
                Each type in c++ has a unique ID that can be seen using the type_info of an specific data.
                For example, std::byte could have the id of '1240149' and it's only used for std::byte data type.
                This is a nice way to store the type of the data without using enums, since for
                each new data type you have to change the enum by hand. If you want to literally store
                hundreds of data types, is convenient not to use enums.
                
                For this C++ feature you need to add the <typeinfo> header.
            */
            idType type_id;
            //! Index of the slotmap
            /*!
                The data will be stored in slotmaps for practical purposes.
                In case you stored more than the maximum capacity of the slotmap, instead of resizing the slotmap we will
                create a new one so the data won't be moved and copied from one point of the heap to another, 
                avoiding memory fragmentation this way.

                Example: if you have 1024 objects/structs inside a slotmap and the maximum capacity is 1024, the previous
                keys will have an index value of '0', but the 1025 to 2048 elements will have index value of '1' and so on. 
            */
            indexSlotmap index;
            //! Key to the slotmap
            /*!
                This is the key that references the current stored data in the specified slotmap. 
            */
            SlotMap::SlotMap_Key key;
        };

        private:
        //! Length of the slotmap
        /*!
            The purpose of this struct is to make easier configurations in compilation-time.
            The default slotmap length per value is 1024, but if you need another size, you can edit this file freely
            and either change the default one or use template specialization.

            Example of how to make class specification:
            \code{.cpp}
            template<typename Resource_Stored>
            struct slotmap_length {
                static constexpr unsigned int value = 1024;
            };

            template<>
            struct slotmap_length<unsigned long long>{
                static constexpr unsigned int value = 128;
            }
            \endcode
            
            @tparam Resource_Stored Data type
            @sa SlotMapAlloc
        */
        template<typename Resource_Stored>
        struct slotmap_length {
            static constexpr unsigned int value = 1024;
        };

        //! SlotMap Allocator
        /*!
            This is the allocator that reserves bytes in the heap so the slotmaps can store data inside this. 
            The allocation will have enough size to store 4 slotmaps (in this case, 1024*4 elements).
            @tparam Resource_Stored Data type
            @sa SlotMap
            @sa Static_Pool_Allocator

        */
        template<typename Resource_Stored>
        using SlotMapAlloc = RandysEngine::Pool::Static_pool_allocator<Resource_Stored,4*(slotmap_length<Resource_Stored>::value+1)>;

        //! SlotMap Type
        /*!
            A shortened name for the specific slotmap that the user needs.
            @tparam Resource_Stored Data type
            @sa SlotMap
            @sa Static_Pool_Allocator
        */
        template<typename Resource_Stored>
        using SlotMapType = RandysEngine::SlotMap::SlotMap<Resource_Stored,SlotMapAlloc<Resource_Stored>>;
        //! SlotMap List Data
        /*!
            Configuration data for the slotmap list.
            Note: if you specialize this class, NEVER EVER use slotmaps_number under value 16 
            because that's the minimun size for std::DEQUE, which is implemented in SlotMapList class
            @tparam Resource_Stored Data type
            @sa SlotMap
            @sa Static_Pool_Allocator
            @sa ListAlloc
            @sa baseList
            @sa SlotMapList
        */
        template<typename Resource_Stored>
        struct SlotMapListData{
            //NOTE: if you specialize this class, NEVER EVER use slotmaps_number under value 16 
            //because that's the minimun size for std::DEQUE
            static constexpr unsigned int slotmaps_number = 16;
            //the next id of the next slotmap and element
            static inline indexSlotmap nextMap{0};
        };
        //! List allocator
        /*!
            A shortened version of the List allocator, using the configuration Data inside SlotMapListData
            @tparam Resource_Stored Data type
            @sa SlotMap
            @sa Static_Pool_Allocator
            @sa SlotMapListData
            @sa baseList
            @sa SlotMapList
        */
        template<typename Resource_Stored>
        using ListAlloc = RandysEngine::Pool::Static_pool_allocator<
            SlotMapType<Resource_Stored>, 
            SlotMapListData<Resource_Stored>::slotmaps_number
        >;

        //! List base class
        /*!
            All the lists will be specialized as a subclass.
            @sa SlotMapList
        */
        struct baseList{

            baseList(){};

            virtual ~baseList(){};

        };

        //! Slotmap List
        /*!
            This is a list of slotmaps of the specialized data types.
            @sa baseList
        */
        template<typename Resource_Stored>
        struct SlotMapList : baseList {
            std::deque<SlotMapType<Resource_Stored>,ListAlloc<Resource_Stored>> list;
        
            ~SlotMapList(){};
        };

        //! Map of lists
        /*!
            All of the slotmap lists will be stored inside this std::map in which the key is the id of the slotmaplist type
            and the pointer is a casted to base class of the specific slotmap list.
            @sa baseList
        */
        std::map<std::size_t,
            baseList*> listMap;

        //! Map of lists
        /*!
            This function is used to get a reference to the slotmaplist while casting it.
            If the list doesn't exists, then it will create it.
            @return reference to the slotmaplist
            @sa SlotMapList
        */
        template<typename Resource_Stored>
        SlotMapList<Resource_Stored>& getSlotMapList() noexcept{
            
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

            //! Constructor
            /*!
                Only this constructor is allowed.
            */
            ResourceManager() noexcept{};

            //! Destructor
            /*!
                This destructor will all of the lists and call to their destructor without casting them since the baseList destructor is virtual.
                @sa SlotMapList
            */
            ~ResourceManager() noexcept{
                for(auto& pair : listMap){
                    std::unique_ptr<baseList>(pair.second);
                }
            };

            //! Reserve resource
            /*!
                Store an existing resource inside the resource manager.
                Use this function for storing objects or structs without destructors.
                Rvalue version.
                @tparam Resource_Stored data type.
                @param resource resource to be stored.
                @return key that references to the new stored data.
                @sa createResource
                @sa getResource
                @sa freeResource
            */
            template<typename Resource_Stored>
            [[nodiscard]] const KeyId reserveResource(Resource_Stored&& resource) noexcept{
                
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

            //! Reserve resource
            /*!
                Store an existing resource inside the resource manager.
                Use this function for storing objects or structs without destructors.
                Lvalue version.
                @tparam Resource_Stored data type.
                @param resource resource to be stored.
                @return key that references to the new stored data.
                @sa createResource
                @sa getResource
                @sa freeResource
            */
            template<typename Resource_Stored>
            [[nodiscard]] const KeyId reserveResource(Resource_Stored& resource) noexcept{
                return(this->reserveResource(Resource_Stored{resource}));
            }
            //! Create resource.
            /*!
                Creates an object and stores it inside the resource manager.
                Use this function for storing objects or structs with destructors.
                @tparam Resource_Stored data type.
                @tparam Args types of the constructor arguments for the data type.
                @param input argument data
                @return key that references to the new stored data
                @sa reserveResource
                @sa getResource
                @sa freeResource
            */
            template<typename Resource_Stored,class... Args>
            [[nodiscard]] const KeyId createResource(Args... input) noexcept{
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

            //! Get resource.
            /*!
                Gets a resource that is previously stored using the key.
                @tparam Resource_Stored data type.
                @param input Key that references a stored resource.
                @return pointer to the memory location of the resource.
                @sa reserveResource
                @sa createResource
                @sa freeResource
            */
            template<typename Resource_Stored>
            Resource_Stored* getResource(const KeyId input) noexcept{
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

            //! Free resource.
            /*!
                Frees a resource using the specific key. Then the key becomes invalid.
                @tparam Resource_Stored data type.
                @param input Key that references a stored resource.
                @return result of the operation. True if the resource has been freed succesfully, otherwise false.
                @sa reserveResource
                @sa createResource
                @sa getResource
            */
            template<typename Resource_Stored>
            bool freeResource(KeyId input) noexcept{
                bool devolver = false;
                //If this is the right resource type
                if(input.type_id == typeid(Resource_Stored).hash_code()){
                    //get the list
                    auto& SlotMapList = getSlotMapList<Resource_Stored>().list;
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

    /*! @} */


};