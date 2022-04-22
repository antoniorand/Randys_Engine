//Source: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0661r0.pdf

#pragma once
#include "../memoryPool/memoryPool.hpp"

#include <cstddef>
#include <memory>
#include <array>
#include <exception>
#include <cassert>


namespace RandysEngine{

        //We will be using the std::allocator by default, 
        //but you can use other allocs as well as the defined by your
        
        template<class Type,class DATA_ALLOC = std::allocator<Type>>
        class SlotMap{
            /////
            public:
                //Type of the Index to be stored
                using Index_Type    = std::uint32_t;

            private:

                //Some configurations must be provided

                //Type of the value to be stored
                using Value_Type    = Type;

                //Type of the Id to be stored
                using Id_Type       = Index_Type;

                //Type of the Generation to be stored
                using Gen_Type      = std::uint64_t;

            public:
                //Type of the key to be returned and stored
                using Key           = struct{Id_Type Id;Gen_Type Gen;};
            private:

                //Type of the data array
                using TypeArray     = Type*;

                //Type of the indices array (each of the indices are made of a key)
                using IndicesArray  = Key*;

                //Type of the erase array
                using EraseArray    = Index_Type*;

                //Rebinding of the allocator provided
                using INDICES_ALLOC = typename std::allocator_traits<DATA_ALLOC>::template rebind_alloc<Key>;
                
                //Rebinding of the allocator provided to erase type
                using ERASE_ALLOC   = typename std::allocator_traits<DATA_ALLOC>::template rebind_alloc<Index_Type>;
            /////

            //Array of data to be stored by the allocator
            TypeArray                     Data = nullptr;

            //Index that indicates which is the next free element
            Index_Type                    Free_list_head{0};

            //Index that indicates the number of elements currently stored by the slotmap
            Index_Type                    Size{0};

            //Index that indicates the number of max elements stored by the slotmap
            Index_Type                    Capacity;

            //Array of Erase elements
            EraseArray                    Erase = nullptr;

            //Index of indices that are currently stored by the slotmap
            IndicesArray                  Indices = nullptr;

            //Data allocator
            DATA_ALLOC d_alloc{};

            //Indices allocator
            INDICES_ALLOC i_alloc{};

            //Erase allocator
            ERASE_ALLOC e_alloc{};

            //Current generation that is right now in the Slot_Map
            Gen_Type                      Generation{0};

            //Initialization of the freelist.
            //There is no need to delete the data stored in the slotmap since
            //the data is initialized beforehand
            constexpr void freelist_init() noexcept{

                //Reset all of the indices, we don't care about the gen_type
                for(Index_Type i = 0; i < Capacity;i++){
                    Indices[i].Id = i+1;
                }
                //Put freelist to zero
                Free_list_head = 0;
            }

            [[nodiscard]] constexpr Index_Type allocate() {
                
                if(Size >= Capacity) {

                    //Increment data, index and erase array by 128 and copy data
                    Index_Type newCapacity = Capacity+128;

                    //Allocate new memory
                    TypeArray newData = d_alloc.allocate(newCapacity);
                    IndicesArray newIndices = i_alloc.allocate(newCapacity);
                    EraseArray newErase = e_alloc.allocate(newCapacity);    

                    //Copy new memory from old memory
                    memcpy(newData,Data,sizeof(Value_Type)*Capacity);
                    memcpy(newIndices,Indices,sizeof(Key)*Capacity);
                    memcpy(newErase,Erase,sizeof(Index_Type)*Capacity);

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

            constexpr void free(Key key) noexcept{

                //we look if the key is valid
                //is unimportant in this specific implementation
                //assert(isValid(key));

                //get the slot specified in the key
                auto& slot = Indices[key.Id];
                //Get the index to the data array
                auto dataid = slot.Id;//save id of data slot to check if it is last or not

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

            public:
                //Initializing the SlotMap and reserve new Data
                constexpr explicit SlotMap(Index_Type e_capacity = 128) : Capacity {e_capacity}{

                    Data = d_alloc.allocate(Capacity);
                    Indices = i_alloc.allocate(Capacity);
                    Erase = e_alloc.allocate(Capacity);   
                    freelist_init();
                }

                //Constructor de copia borrado
                template<class Type_External,class Allocator_External>
                constexpr explicit SlotMap(const SlotMap<Type_External,Allocator_External>& other) = delete;

                //Destructing the SlotMap
                constexpr ~SlotMap(){
                    d_alloc.deallocate(Data,Capacity);
                    i_alloc.deallocate(Indices,Capacity);
                    e_alloc.deallocate(Erase,Capacity);  
                }                

                //Push a new value into the last free position of slotmap
                [[nodiscard]] Key push_back(Value_Type&& input){
                    
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

                [[nodiscard]] Key push_back(Value_Type const& input){
                    return(this->push_back(Value_Type{input}));
                }

                template<class... Args>
                [[nodiscard]] Key push_back(Args... input){
                    return(this->push_back(Value_Type{input...}));
                }

                void swap(SlotMap<Value_Type,DATA_ALLOC>& other) noexcept{
                    std::swap(Data,other.Data);
                    std::swap(Free_list_head,other.Free_list_head);
                    std::swap(Size,other.Size);
                    std::swap(Capacity,other.Capacity);
                    std::swap(Erase,other.Erase);
                    std::swap(Generation,other.Generation);
                    std::swap(Indices,other.Indices);                    
                }

                constexpr void clear() noexcept{ freelist_init();}

                constexpr bool resize(Index_Type newCapacity) {
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
                        memcpy(newData,Data,sizeof(Value_Type)*Capacity);
                        memcpy(newIndices,Indices,sizeof(Key)*Capacity);
                        memcpy(newErase,Erase,sizeof(Index_Type)*Capacity);

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

                constexpr void clear_and_resize(Index_Type newCapacity){

                    //Allocate new memory
                    TypeArray newData = d_alloc.allocate(newCapacity);
                    IndicesArray newIndices = i_alloc.allocate(newCapacity);
                    EraseArray newErase = e_alloc.allocate(newCapacity);    

                    //Dealloc old memory
                    d_alloc.deallocate(Data,Capacity);
                    i_alloc.deallocate(Indices,Capacity);
                    e_alloc.deallocate(Erase,Capacity);

                    //Replace pointers
                    Data = newData;   
                    Indices = newIndices;
                    Erase = newErase;
                    Capacity = newCapacity;    
                    clear();
                }

                constexpr bool erase(Key key) noexcept{
                    bool devolver = isValid(key);
                    if(devolver){
                        this->free(key);
                    }
                    return devolver;
                }

                [[nodiscard]] constexpr bool isValid(Key key) noexcept{
                    bool devolver = true;
                    if(key.Id >= Capacity 
                    || Indices[key.Id].Gen != key.Gen)
                        devolver = false;

                    return(devolver);
                }
            
                [[nodiscard]] constexpr std::size_t current_size() const noexcept{
                    return Size;
                }

                [[nodiscard]] constexpr std::size_t max_capacity() const noexcept{
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
                
                Type* atPosition(std::uint32_t input){
                    Type* devolver = nullptr;
                    if(input < Size){
                        devolver = &Data[input];
                    }
                    return (devolver);
                }

                Type* atPosition(Key input){
                    Type* devolver = nullptr;
                    if(isValid(input)){
                        devolver = &Data[
                            Indices[input.Id].Id
                        ];
                    }
                    return(devolver);
                }

                //constexpr Index_Type getFreeList(){return Free_list_head;};

        };
    

}