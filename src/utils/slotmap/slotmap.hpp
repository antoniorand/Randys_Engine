//Source: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0661r0.pdf

#pragma once
#include <memoryPool/memoryPool.hpp>

#include <cstddef>
#include <memory>
#include <array>
#include <exception>
#include <cassert>


namespace RandysEngine{

    namespace SlotMap{

        //We will be using the std::allocator by default
        template<class Type, std::uint32_t Capacity = 100,class Allocator = std::allocator<Type>>
        class SlotMap{
            /////
            private:
                using Value_Type    = Type;
                using Index_Type    = std::uint32_t;
                using Id_Type       = Index_Type;
                using Gen_Type      = Index_Type;
            public:
                using Key           = struct{Id_Type Id;Gen_Type Gen;};
            private:
                using IndicesArray  = std::array<Key,Capacity>;
                using EraseArray    = std::array<Index_Type,Capacity>;
            //using INDICES_ALLOC = typename std::allocator_traits<Allocator>::template rebind_alloc<std::pair<Slot,std::size_t>>;
            //using ERASE_ALLOC   = typename std::allocator_traits<Allocator>::template rebind_alloc<std::size_t>;
            /////
            Type*                         Data = nullptr;
            Index_Type                    Free_list_head{0};
            //Index_Type                    Free_list_tail{Capacity-1};
            Index_Type                    Size{0};
            Gen_Type                      Generation{0};
            Allocator                     Alloc{};
            IndicesArray                  Indices;
            EraseArray                    Erase;
            
            constexpr void freelist_init() noexcept{
                for(Index_Type i = 0; i < Indices.size();i++){
                    Indices[i].Id = i+1;
                }
                Free_list_head = 0;
            }

            [[nodiscard]] constexpr Index_Type allocate() {
                
                if(Size >= Capacity) throw std::runtime_error("Can't allocate in Slot_Map");
                assert(Free_list_head< Capacity);

                //Reserve
                auto slotid = Free_list_head;
                Free_list_head = Indices[slotid].Id;

                //Init slot
                auto& slot = Indices[slotid];
                slot.Id = Size;
                slot.Gen = Generation;

                //update space and generation
                Size+=1;
                Generation+=1;
                return slotid;
            }

            constexpr void free(Key key) noexcept{
                assert(isValid(key));

                auto& slot = Indices[key.Id]; 
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

                constexpr explicit SlotMap(){
                    Data = Alloc.allocate(Capacity);
                    freelist_init();
                }
                constexpr explicit SlotMap(const SlotMap<Type,Capacity,Allocator>& other) = delete;

                constexpr ~SlotMap(){
                    Alloc.deallocate(Data,Capacity);
                }                
                //Push a new value into the last free position of slotmap
                [[nodiscard]] Key push_back(Value_Type&& input){
                    //Allocate a new slot and return the position of the slot (not the data)
                    auto reservedId = allocate();
                    //get the reserved slot
                    auto key = Indices[reservedId];

                    //move data
                    Data[key.Id] = std::move(input);
                    Erase[key.Id] = reservedId;

                    key.Id = reservedId;                    
                    return{key};
                }

                [[nodiscard]] Key push_back(Value_Type const& input){
                    return(this->push_back(Value_Type{input}));
                }

                template<class... Args>
                [[nodiscard]] Key push_back(Args... input){
                    return(this->push_back(Value_Type{input...}));
                }

                void swap(SlotMap<Type,Capacity,Allocator>& other) noexcept{
                    {
                        Type* aux = other.Data;
                        other.Data = this->Data;
                        this->Data = aux;
                    }
                    {
                        Index_Type aux = other.Free_list_head;
                        other.Free_list_head = this->Free_list_head;
                        this->Free_list_head = aux;
                    }
                    {
                        Gen_Type aux = other.Generation;
                        other.Generation = this->Generation;
                        this->Generation = aux;
                    }
                    {
                        Allocator aux = other.Alloc;
                        other.Alloc = this->Alloc;
                        this->Alloc = aux;
                    }
                    std::swap(Indices,other.Indices);
                    std::swap(Erase,other.Erase);
                }

                constexpr void clear() noexcept{ freelist_init();}

                constexpr bool erase(Key key) noexcept{
                    bool devolver = isValid(key);
                    if(devolver){
                        free(key);
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

                constexpr bool operator==(const SlotMap<Type,Capacity,Allocator>&other) const = delete;

                constexpr bool operator!=(const SlotMap<Type,Capacity,Allocator>&other) const = delete;

                constexpr bool operator<(const SlotMap<Type,Capacity,Allocator>&other) const = delete;

                constexpr bool operator<=(const SlotMap<Type,Capacity,Allocator>&other) const = delete;

                constexpr bool operator>(const SlotMap<Type,Capacity,Allocator>&other) const = delete;

                constexpr bool operator>=(const SlotMap<Type,Capacity,Allocator>&other) const = delete;

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
    };

}