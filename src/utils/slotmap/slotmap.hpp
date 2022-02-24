//Source: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0661r0.pdf

#pragma once
#include <memoryPool/memoryPool.hpp>

#include <cstddef>
#include <memory>
#include <array>


namespace RandysEngine{

    namespace SlotMap{

        //We will be using the std::allocator by default
        template<class Type, std::uint32_t Capacity,class Allocator = std::allocator<Type>>
        class SlotMap{
            /////
            private:
                using Value_Type    = Type;
                using Index_Type    = std::uint32_t;
                using Id_Type       = Index_Type;
                using Gen_Type      = Index_Type;
            public:
                using Slot          = struct{Id_Type Id;Gen_Type Gen;};
            private:
                using IndicesArray  = std::array<Index_Type,Capacity>;
                using EraseArray    = std::array<Index_Type,Capacity>;
            //using INDICES_ALLOC = typename std::allocator_traits<Allocator>::template rebind_alloc<std::pair<Slot,std::size_t>>;
            //using ERASE_ALLOC   = typename std::allocator_traits<Allocator>::template rebind_alloc<std::size_t>;
            /////
            Type*                         Data = nullptr;
            Index_Type                    Free_list_head{0};
            //Index_Type                    Free_list_tail{Capacity-1};
            Index_Type                    Size{0};
            Allocator                     Alloc{};
            IndicesArray                  Indices;
            EraseArray                    Erase;
            
            constexpr void freelist_init() noexcept{
                for(Index_Type i = 0; i < Indices.size();i++){
                    Indices[i] = i+1;
                }
                Free_list_head = 0;
            }

            public:

                constexpr explicit SlotMap(){
                    Data = Alloc.allocate(Capacity);
                    freelist_init();
                }
                constexpr ~SlotMap(){
                    Alloc.deallocate(Data,Capacity);
                }

                //Tengo que aplicar el perfecto forwarding TO-DO

                template<class... Args>
                [[nodiscard]] Slot emplace_back(Args... input);
                
                [[nodiscard]] Slot push_back(Value_Type&& input){
                    return{};
                }

                
                void swap(SlotMap<Type,Capacity,Allocator>& other) noexcept;

                constexpr void clear() noexcept;

                constexpr void erase(std::size_t pos);
                
                constexpr void erase(std::size_t initial, std::size_t final);

                [[nodiscard]] constexpr std::size_t current_size() const noexcept{
                    return Size;
                }

                [[nodiscard]] constexpr std::size_t max_capacity() const noexcept{
                    return Capacity;
                }

                constexpr bool operator==(const SlotMap<Type,Capacity,Allocator>&other) const;

                constexpr bool operator!=(const SlotMap<Type,Capacity,Allocator>&other) const;

                constexpr bool operator<(const SlotMap<Type,Capacity,Allocator>&other) const;

                constexpr bool operator<=(const SlotMap<Type,Capacity,Allocator>&other) const;

                constexpr bool operator>(const SlotMap<Type,Capacity,Allocator>&other) const;

                constexpr bool operator>=(const SlotMap<Type,Capacity,Allocator>&other) const;

        };
    };

}