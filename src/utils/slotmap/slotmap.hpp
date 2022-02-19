//Source: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0661r0.pdf

#pragma once
#include <memoryPool/memoryPool.hpp>


namespace RandysEngine{

    namespace SlotMap{

        //We will be using the std::allocator by default
        template<class Type, class Allocator = std::allocator<Type>>
        class SlotMap{
            std::size_t* Indices;
            std::size_t *Data;
            std::size_t Free_list_head;
            std::size_t Free_list_tail;
            std::size_t capacity;
            std::size_t current_size;
            std::size_t* Erase = nullptr;
            Allocator alloc{};
            public:

            SlotMap(std::size_t e_capacity);

        };
    };

}