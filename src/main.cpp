
#include <memoryPool.hpp>
#include <iostream>

#define CHECK_BIT_REVERSED(var,pos) (((uint8_t)var) & (0x80 >> pos))
#define SET_BIT_REVERSED  (var,pos) (((uint8_t)var) | (0x80 >> pos))  

int main(){ 


    auto poolilla = RandysEngine::Pool::Bucket {100,10};

    auto ptr1 = poolilla.allocate(100);
    auto ptr2 = poolilla.allocate(100);
    auto ptr3 = poolilla.allocate(100);
    poolilla.deallocate(ptr2,100);
    auto ptr4 = poolilla.allocate(100);

    return 0;
}