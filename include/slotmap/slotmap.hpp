/**
 * @file memoryPool.hpp
 * @author Antonio Alberto Peguero Lopez (tony.randalph@gmail.com)
 * @brief Memory Pool implementation with C++ and STL functions
 * @version 1.0
 * @date 2022-08-15
 * @copyright Copyright (c) 2022
 * 
 */

//Implementation of this specification: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0661r0.pdf

#pragma once
#include "../memoryPool/memoryPool.hpp"

#include <cstddef>
#include <memory>
#include <array>
#include <exception>
#include <cassert>


namespace RandysEngine{

    namespace SlotMap{

        /*! \defgroup SlotMap_namespace SlotMap
        *  Bucket class documentation
        *  @{
        */

        //! Type of the Index to be stored in the heap.
        /*!
            @sa SlotMap_Id_Type
        */
        using SlotMap_Index_Type    = std::uint32_t;
        //! Type of the Id in the key.
        /*!
            @sa SlotMap_Index_Type
            @sa SlotMap_Gen_Type
            @sa SlotMap_Key
        */
        using SlotMap_Id_Type       = SlotMap_Index_Type;
        //! Type of the Gen in the key.
        /*!
            @sa SlotMap_Id_Type
            @sa SlotMap_Key
        */
        using SlotMap_Gen_Type      = std::uint64_t;
        //! Type of the key that references stored data.
        /*!
            @sa SlotMap_Id_Type
            @sa SlotMap_Key
        */
        struct SlotMap_Key{
            RandysEngine::SlotMap::SlotMap_Id_Type Id;
            RandysEngine::SlotMap::SlotMap_Gen_Type Gen;
            bool operator ==(const SlotMap_Key &b) const{
                return((Id == b.Id) && (Gen == b.Gen));
            }
        };

        //We will be using the std::allocator by default, 
        //but you can use other allocs as well as the defined by you
        
        //!  Class SlotMap
        /*!
            A class that is a data container of elements of the same type, contiguous in memory.
            Works in a similar way as vector internally, but to access the elements, you need a key.
            The key is gotten whenever you store a new element, at it gets you a reference to that element.
            The key is also used to delete the elements stored in the slotmap.
            That's why the key has to be assigned to a variable when returned, otherwise you will get a compiling error.
            @tparam Type type of the data to be stored.
            @tparam DATA_ALLOC allocator that manages storing in the heap
        */
        template<class Type,class DATA_ALLOC = std::allocator<Type>>
        class SlotMap{

                //Some configurations must be provided

                //! Type of the value to be stored.
                /*!
                    @sa TypeArray
                */
                using Value_Type    = Type;
                //! Type of the data array.
                /*!
                    @sa Value_Type
                    @sa IndicesArray
                    @sa EraseArray
                */
                using TypeArray     = Type*;
                //! Type of the indices array.
                /*!
                    @sa Value_Type
                    @sa DataArray
                    @sa EraseArray
                */
                using IndicesArray  = SlotMap_Key*;
                //! Type of the erase array.
                /*!
                    @sa Value_Type
                    @sa IndicesArray
                    @sa DataArray
                */
                using EraseArray    = SlotMap_Index_Type*;
                //! Rebinder to an allocator to an Indices Type.
                /*!
                    @sa IndicesArray
                    @sa SlotMap_Key
                */
                using INDICES_ALLOC = typename std::allocator_traits<DATA_ALLOC>::template rebind_alloc<SlotMap_Key>;
                //! Rebinder to an allocator to an Erase Type.
                /*!
                    @sa EraseArray
                */
                using ERASE_ALLOC   = typename std::allocator_traits<DATA_ALLOC>::template rebind_alloc<SlotMap_Index_Type>;
            //! Array of data to be stored by the allocator.
            /*!
                @sa TypeArray
            */
            TypeArray                             Data = nullptr;
            //! Index that indicates which is the next free element.
            /*!
                @sa SlotMap_Index_Type
            */
            SlotMap_Index_Type                    Free_list_head{0};
            //! Number of elements currently stored by the slotmap.
            /*!
                @sa SlotMap_Index_Type
                @sa Capacity
            */
            SlotMap_Index_Type                    Size{0};
            //! Maximum number of elements that the slotmap can store.
            /*!
                @sa SlotMap_Index_Type
                @sa Size
            */
            SlotMap_Index_Type                    Capacity;
            //! Array of erase data to be stored by the allocator.
            /*!
                @sa EraseArray
            */
            EraseArray                    Erase = nullptr;
            //! Array of indices that are currently stored by the slotmap.
            /*!
                @sa IndicesArray
            */
            IndicesArray                  Indices = nullptr;

            //! Instance of the allocator that stores dat in the slotmap.
            /*!
                @sa Value_Type
            */
            DATA_ALLOC d_alloc{};
            //! Instance of the allocator that stores indices in the index array.
            /*!
                @sa SlotMap_Key
                @sa IndicesArray
                @sa DATA_ALLOC
            */
            INDICES_ALLOC i_alloc{};
            //! Instance of the allocator that stores indices in the erase array.
            /*!
                @sa SlotMap_Index_Type
                @sa EraseArray
                @sa INDICES_ALLOC
            */
            ERASE_ALLOC e_alloc{};
            //! Current generation of the Slot_Map.
            /*!
                @sa SlotMap_Gen_Type
            */
            SlotMap_Gen_Type                      Generation{1};

            //! FreeList initialization.
            /*!
                Initialization of the freelist.
                There is no need to delete the data stored since the data is initialized beforehand.
            */
            constexpr void freelist_init() noexcept{

                //Reset all of the indices, we don't care about the gen_type
                for(SlotMap_Index_Type i = 0; i < Capacity;i++){
                    Indices[i].Id = i+1;
                }
                //Put freelist to zero
                Free_list_head = 0;
            }
            //! Allocates memory inside the SlotMap.
            /*!
                Gets one free slot in the slotmap.
                If the maximum capacity is reached, it reserved more memory in the heap and copies the previous
                data in the new memory position.
                @return Index to the position in the slotmap.
                @sa SlotMap_Index_Type
                @sa d_alloc
                @sa e_alloc
                @sa i_alloc
                @sa size
                @sa capacity
                @sa Indices
                @sa Erase
            */
            [[nodiscard]] constexpr SlotMap_Index_Type allocate() {
                
                if(Size >= Capacity) {

                    //Increment data, index and erase array by 128 and copy data
                    SlotMap_Index_Type newCapacity = Capacity+128;

                    //Allocate new memory
                    TypeArray newData = d_alloc.allocate(newCapacity);
                    IndicesArray newIndices = i_alloc.allocate(newCapacity);
                    EraseArray newErase = e_alloc.allocate(newCapacity);    

                    //Copy new memory from old memory
                    std::copy(Data,Data + sizeof(Value_Type)*newCapacity,newData);
                    std::memcpy(newIndices,Indices,sizeof(SlotMap_Key)*Capacity);
                    std::memcpy(newErase,Erase,sizeof(SlotMap_Index_Type)*Capacity);


                    //Dealloc old memory
                    d_alloc.deallocate(Data,Capacity);
                    i_alloc.deallocate(Indices,Capacity);
                    e_alloc.deallocate(Erase,Capacity);

                    //Replace pointers
                    Data = newData;   
                    Indices = newIndices;
                    Erase = newErase;  
                    Capacity = newCapacity;
                }

                //Get a slot from the position specified in the freelist
                auto slotid = Free_list_head;
                //Now the freelist head is the position that points the old free slot
                Free_list_head = Indices[slotid].Id;

                //Get the slot
                auto& slot = Indices[slotid];
                //The new id will be size
                slot.Id = Size;
                //we add the current gen
                slot.Gen = Generation;

                //update space and generation
                Size+=1;
                Generation+=1;
                return slotid;
            }
            //! Frees an element and clears that slot in the slotmap.
            /*!
                From the key that is given, it calls the destructors to the element.
                After this, it updates the freelist and copies the data in the last position of the slotmap
                into the deleted slot.
                @param SlotMap_Key key that needs to be freed.
                @sa SlotMap_Key
                @sa Value_Type
                @sa size
                @sa capacity
                @sa Indices
                @sa Erase
                @sa Data
                @sa Free_list_head
            */
            constexpr void free(SlotMap_Key key) noexcept{

                //we look if the key is valid
                //is unimportant in this specific implementation
                //assert(isValid(key));

                //get the slot specified in the key
                auto& slot = Indices[key.Id];
                //Get the index to the data array
                auto dataid = slot.Id;//save id of data slot to check if it is last or not
                //Destroy the data in the specific place
                auto element = &Data[slot.Id];

                (element)->~Value_Type();

                //Update freelist
                slot.Id = Free_list_head;
                slot.Gen = Generation;
                Free_list_head = key.Id;
                
                //copy data to free slot?
                if(dataid != Size-1){
                    //data slot is not last, copy last here
                    Data[dataid]  = Data[Size-1];
                    Erase[dataid] = Erase[Size-1];
                    Indices[Erase[dataid]].Id = dataid;
                }
                //Update size and generation
                Generation+=1;
                Size-=1;
            }
            //! Destroy all elements.
            /*!
                This function is called only in the destructor of the slotmap or when slotmap is emptied.
                It calls the destructors to the elements stored, if any, but doesn't deallocate
                memory in the heap.
                @param SlotMap_Key key that needs to be freed.
                @sa SlotMap_Key
                @sa size
            */
            constexpr void destroyElements() noexcept{

                for(SlotMap_Index_Type i = 0; i < Size;i++){
                    auto element = &Data[i];
                    (element)->~Value_Type();
                }

            }

            public:
                //! Main constructor
                /*!
                    Allocates memory in the arrays with empty memory.
                    Then it initialized the freelist.
                    @param e_capacity capacity specified by the user, otherwise is 128 by default.
                    @sa d_alloc
                    @sa i_alloc
                    @sa e_alloc
                    @sa freelist_init
                    @sa capacity
                */
                constexpr explicit SlotMap(SlotMap_Index_Type e_capacity = 128) : Capacity {e_capacity}{

                    Data = d_alloc.allocate(Capacity);
                    Indices = i_alloc.allocate(Capacity);
                    Erase = e_alloc.allocate(Capacity);   
                    freelist_init();
                }

                //Constructor de copia borrado
                template<class Type_External,class Allocator_External>
                constexpr explicit SlotMap(const SlotMap<Type_External,Allocator_External>& other) = delete;

                //! Class destructor
                /*!
                    Destroys the elements stored in the slotmap,
                    then it deallocates it.
                    @sa d_alloc
                    @sa i_alloc
                    @sa e_alloc
                    @sa destroyElements
                    @sa capacity
                */
                constexpr ~SlotMap(){
                    destroyElements();
                    d_alloc.deallocate(Data,Capacity);
                    i_alloc.deallocate(Indices,Capacity);
                    e_alloc.deallocate(Erase,Capacity);  
                }                

                //! Push an existing element into the slotmap
                /*!
                    Copies the element right into the slotmap, and assigns a key to it. 
                    The generation is then increased.
                    @param input new element to be pushed inside the slotmap.
                    @return key that references the new element stored in the slotmap.
                    @sa Indices
                    @sa allocate
                    @sa Data
                    @sa Erase
                    @sa SlotMap_Key
                    @sa generation
                */
                [[nodiscard]] const SlotMap_Key push_back(Value_Type&& input) {
                    //Allocate a new slot and return the position of the slot (not the data)
                    auto reservedId = allocate();
                    //get the reserved slot
                    auto key = Indices[reservedId];

                    //move data
                    Data[key.Id] = std::forward<Value_Type>(input);
                    Erase[key.Id] = reservedId;

                    //We change the id of the key to be returned
                    //This is because in the slotmap, the key should point to the index to the array data
                    //but the key outside should point to the index of the indices data
                    key.Id = reservedId;                    

                    //EXAMPLE
                    /*
                        I push back a new element, it is reserved in the 10th position of the index,
                        but it will become the 42nd element in the data array.

                        So in the indices array, the key value would be like [41,156]
                        and the key to be returned is going to be [10,156]

                        So you would tell the slotmap "I want the tenth element please"
                        And the slotmap will see that the 10th element is going to be stored in the forty first
                        element in the data array, so you will be returned the pointer to the forty first
                        but the user won't need to know all of this :) this explanation is just for debugging
                    */
                    return{key};
                }
                //! Push an existing element into the slotmap
                /*!
                    Copies the element right into the slotmap, and assigns a key to it. 
                    The generation is then increased.
                    @param input new element to be pushed inside the slotmap.
                    @return key that references the new element stored in the slotmap.
                    @sa Indices
                    @sa allocate
                    @sa Data
                    @sa Erase
                    @sa SlotMap_Key
                    @sa generation
                */
                [[nodiscard]] const SlotMap_Key push_back(Value_Type const& input){
                    return(this->push_back(Value_Type{input}));
                }
                //! Emplace a new element in the slotmap.
                /*!
                    Creates a new element from it's arguments. If the arguments are incorrect, the program won't compile correctly.
                    This uses a parameter pack to get flexibility for each type possible.
                    After that uses the 'emplacement new' function to construct the object.
                    @tparam Args parameter parck that is compounded by a list of arguments, the Value_Type constructor parameters.
                    @param input values of the arguments of the constructors.
                    @return key that references the new element stored in the slotmap.
                    @sa Indices
                    @sa allocate
                    @sa Data
                    @sa Erase
                    @sa SlotMap_Key
                    @sa generation
                    @sa push_back
                */
                template<class... Args>
                [[nodiscard]] const SlotMap_Key emplace_back(Args... input){
                    //Allocate a new slot and return the position of the slot (not the data)
                    auto reservedId = allocate();
                    //get the reserved slot
                    auto key = Indices[reservedId];

                    //move data
                    new (&Data[key.Id]) Value_Type(input...);
                    Erase[key.Id] = reservedId;

                    //We change the id of the key to be returned
                    //This is because in the slotmap, the key should point to the index to the array data
                    //but the key outside should point to the index of the indices data
                    key.Id = reservedId;     

                    return(key);
                }
                //! Swaps the data from one slotmap to another
                /*!
                    @param other reference to a slotmap of the same type
                    @sa Data
                    @sa Free_list_head
                    @sa Size
                    @sa Capacity
                    @sa Erase
                    @sa Generation
                    @sa Indices
                */
                void swap(SlotMap<Value_Type,DATA_ALLOC>& other) noexcept{
                    std::swap(Data,other.Data);
                    std::swap(Free_list_head,other.Free_list_head);
                    std::swap(Size,other.Size);
                    std::swap(Capacity,other.Capacity);
                    std::swap(Erase,other.Erase);
                    std::swap(Generation,other.Generation);
                    std::swap(Indices,other.Indices);                    
                }
                //! Clears and empties the elements in the slotmap.
                /*!
                    @sa destroyElements
                    @sa freelist_init
                */
                constexpr void clear() noexcept{ 
                    destroyElements();
                    freelist_init();
                }
                //! Resize the slotmap with a new capacity
                /*!
                    Resizes the slotmap and copies the elements into a new memory position
                    @param newCapacity new capacity of the slotmap
                    @return returns false if the capacity is invalid, otherwise true.
                    @sa capacity
                    @sa TypeArray
                    @sa IndicesArray
                    @sa EraseArray
                    @sa d_alloc
                    @sa i_alloc
                    @sa e_alloc
                */
                constexpr bool resize(SlotMap_Index_Type newCapacity) {
                    bool devolver = true;
                    if(newCapacity <= Capacity){
                        devolver = false;
                    }
                    else{

                        //Allocate new memory
                        TypeArray newData = d_alloc.allocate(newCapacity);
                        IndicesArray newIndices = i_alloc.allocate(newCapacity);
                        EraseArray newErase = e_alloc.allocate(newCapacity);    

                        //Copy new memory from old memory
                        std::copy(Data,Data + sizeof(Value_Type)*newCapacity,newData);
                        std::memcpy(newIndices,Indices,sizeof(SlotMap_Key)*Capacity);
                        std::memcpy(newErase,Erase,sizeof(SlotMap_Index_Type)*Capacity);

                        //Dealloc old memory
                        d_alloc.deallocate(Data,Capacity);
                        i_alloc.deallocate(Indices,Capacity);
                        e_alloc.deallocate(Erase,Capacity);

                        //Replace pointers
                        Data = newData;   
                        Indices = newIndices;
                        Erase = newErase;
                        Capacity = newCapacity;
                    }
                    return(devolver);
                }
                //! Clears and resizes the slotmap with a new capacity
                /*!
                    Clears and resizes the slotmap.
                    @param newCapacity new capacity of the slotmap.
                    @sa capacity
                    @sa TypeArray
                    @sa IndicesArray
                    @sa EraseArray
                    @sa d_alloc
                    @sa i_alloc
                    @sa e_alloc
                    @sa clear
                */
                constexpr void clear_and_resize(SlotMap_Index_Type newCapacity){

                    //Allocate new memory
                    TypeArray newData = d_alloc.allocate(newCapacity);
                    IndicesArray newIndices = i_alloc.allocate(newCapacity);
                    EraseArray newErase = e_alloc.allocate(newCapacity);    

                    //Clears old data
                    clear();

                    //Dealloc old memory
                    d_alloc.deallocate(Data,Capacity);
                    i_alloc.deallocate(Indices,Capacity);
                    e_alloc.deallocate(Erase,Capacity);

                    //Replace pointers
                    Data = newData;   
                    Indices = newIndices;
                    Erase = newErase;
                    Capacity = newCapacity;    
                }
                //! Deletes an element in the slotmap
                /*!
                    Deletes an element in the slotmap
                    @param newCapacity new capacity of the slotmap.
                    @return false if the key is invalid, otherwise returns true.
                    @sa SlotMap_Key
                    @sa free
                    @sa isValid
                */
                constexpr bool erase(SlotMap_Key key) noexcept{
                    bool devolver = isValid(key);
                    if(devolver){
                        this->free(key);
                    }
                    return devolver;
                }
                //! Checks if the key is valid
                /*!
                    @param key key to be checked.
                    @return false if the key is invalid, otherwise returns true.
                    @sa SlotMap_Key
                    @sa Indices
                */
                [[nodiscard]] constexpr const bool isValid(SlotMap_Key key) const noexcept{
                    bool devolver = true;
                    if(key.Id >= Capacity 
                    || Indices[key.Id].Gen != key.Gen)
                        devolver = false;

                    return(devolver);
                }
                //! Returns the value of the size.
                /*!
                    @return number of elements in the slotmap
                    @sa SlotMap_Index_Type
                    @sa Size
                */
                [[nodiscard]] constexpr const SlotMap_Index_Type current_size() const noexcept{
                    return Size;
                }
                //! Returns the value of the Capacity.
                /*!
                    @return maximum capacity of elements in the slotmap
                    @sa SlotMap_Index_Type
                    @sa capacity
                */
                [[nodiscard]] constexpr const SlotMap_Index_Type max_capacity() const noexcept{
                    return Capacity;
                }

                template<class Type_External,class Allocator_External>
                constexpr bool operator=(const SlotMap<Type_External,Allocator_External>&other) const = delete;

                template<class Type_External,class Allocator_External>
                constexpr bool operator==(const SlotMap<Type_External,Allocator_External>&other) const = delete;

                template<class Type_External,class Allocator_External>
                constexpr bool operator!=(const SlotMap<Type_External,Allocator_External>&other) const = delete;

                template<class Type_External,class Allocator_External>
                constexpr bool operator<(const SlotMap<Type_External,Allocator_External>&other) const = delete;

                template<class Type_External,class Allocator_External>
                constexpr bool operator<=(const SlotMap<Type_External,Allocator_External>&other) const = delete;

                template<class Type_External,class Allocator_External>
                constexpr bool operator>(const SlotMap<Type_External,Allocator_External>&other) const = delete;

                template<class Type_External,class Allocator_External>
                constexpr bool operator>=(const SlotMap<Type_External,Allocator_External>&other) const = delete;

                /*void printIndices(){
                    for(unsigned int i = 0; i< Capacity;i++){
                        std::cout << "Indice nº" << i << ": Id " <<Indices[i].Id << " Gen " << Indices[i].Gen << std::endl;
                    }
                }*/

                //! Gets a pointer to the element specified by the number
                /*!
                    @param input of the element in order (ex: number 3)
                    @return pointer to the element
                    @sa Value_Type
                    @sa SlotMap_Index_Type
                    @sa Data
                */
                Value_Type* atPosition(SlotMap_Index_Type input) const noexcept{
                    Type* devolver = nullptr;
                    if(input < Size){
                        devolver = &Data[input];
                    }
                    return (devolver);
                }
                //! Gets a pointer to the element specified by the key
                /*!
                    @param input key that references the element
                    @return pointer to the element
                    @sa Value_Type
                    @sa SlotMap_Key
                    @sa Data
                */
                Value_Type* atPosition(SlotMap_Key input) const noexcept{
                    Type* devolver = nullptr;
                    if(isValid(input)){
                        devolver = &Data[
                            Indices[input.Id].Id
                        ];
                    }
                    return(devolver);
                }

        };

        /*! @} */

    }

       
    

}