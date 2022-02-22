//Source: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0661r0.pdf

#pragma once
//#include <memoryPool/memoryPool.hpp>

#include <cstddef>
#include <memory>


namespace RandysEngine{

    namespace SlotMap{

        struct Slot{
            std::size_t Id;
            std::size_t Gen;
            Slot() = default;
        };

        //We will be using the std::allocator by default
        template<class Type, class Allocator = std::allocator<Type>>
        class SlotMap{

            std::pair<Slot,std::size_t>*  Indices = nullptr;
            Type*                         Data = nullptr;
            std::size_t                   Capacity;
            std::size_t                   Free_list_head{0};
            std::size_t                   Free_list_tail{Capacity-1};
            std::size_t                   Current_size{0};
            std::size_t*                  Erase = nullptr;
            Allocator                     Alloc{};
            using INDICES_ALLOC = typename std::allocator_traits<Allocator>::template rebind_alloc<std::pair<Slot,std::size_t>>;
            using ERASE_ALLOC   = typename std::allocator_traits<Allocator>::template rebind_alloc<std::size_t>;
            public:

                SlotMap(std::size_t e_capacity) : Capacity{e_capacity}{
                    auto rebindedIndices = INDICES_ALLOC(Alloc);
                    auto rebindedDeletes = ERASE_ALLOC(Alloc);
                    Data = Alloc.allocate(Capacity);
                    Indices = rebindedIndices.allocate(Capacity);
                    Erase = rebindedDeletes.allocate(Capacity);
                }
                ~SlotMap(){
                    auto rebindedIndices = INDICES_ALLOC(Alloc);
                    auto rebindedDeletes = ERASE_ALLOC(Alloc);
                    Alloc.deallocate(Data,Capacity);
                    rebindedIndices.deallocate(Indices,Capacity);
                    rebindedDeletes.deallocate(Erase,Capacity);
                }

                //Tengo que aplicar el perfecto forwarding TO-DO

                template<class... Args>
                [[nodiscard]] Slot emplace_back(Args... input);
                template<class... Args>
                [[nodiscard]] Slot emplace_back(Type&& input);

                
                void swap(SlotMap<Type,Allocator>& other) noexcept;

                constexpr void realloc();

                constexpr void clear() noexcept;

                constexpr void erase(std::size_t pos);
                
                constexpr void erase(std::size_t initial, std::size_t final);

                const std::size_t current_size() const noexcept{
                    return current_size;
                }

                const std::size_t max_capacity() const noexcept{
                    return Capacity;
                }

                constexpr bool operator==(const SlotMap<Type,Allocator>&other) const;

                constexpr bool operator!=(const SlotMap<Type,Allocator>&other) const;

                constexpr bool operator<(const SlotMap<Type,Allocator>&other) const;

                constexpr bool operator<=(const SlotMap<Type,Allocator>&other) const;

                constexpr bool operator>(const SlotMap<Type,Allocator>&other) const;

                constexpr bool operator>=(const SlotMap<Type,Allocator>&other) const;

        };
    };

}