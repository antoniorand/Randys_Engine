//Source: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0661r0.pdf

#pragma once
#include <memoryPool/memoryPool.hpp>


namespace RandysEngine{

    namespace SlotMap{

        struct Slot{
            std::size_t Id;
            std::size_t Gen;
            Slot() = default;
        };

        //We will be using the std::allocator by default
        template<class Type, std::size_t Capacity,class Allocator = std::allocator<Type>>
        class SlotMap{

            Slot*           Indices = nullptr;
            Type*           Data = nullptr;
            std::size_t     Free_list_head{0};
            std::size_t     Current_size;
            std::size_t*    Erase = nullptr;
            Allocator       alloc{};
            public:

                SlotMap(std::size_t e_capacity);
                ~SlotMap();

                //Tengo que aplicar el perfecto forwarding TO-DO

                template<class... Args>
                [[nodiscard]] Slot emplace_back(Args... input);
                template<class... Args>
                [[nodiscard]] Slot emplace_back(Type&& input);

                
                void swap(SlotMap<Type,Capacity,Allocator>& other) noexcept;

                constexpr void clear() noexcept;

                constexpr void erase(std::size_t pos);
                
                constexpr void erase(std::size_t initial, std::size_t final);

                const std::size_t current_size() const noexcept{
                    return current_size;
                }

                const std::size_t max_capacity() const noexcept{
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