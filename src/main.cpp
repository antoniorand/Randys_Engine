
//////

/////
#include <memoryPool/memoryPool.hpp>
#include <slotmap/slotmap.hpp>
#include <vector>
#include <iostream>

struct Estructura{
    std::uint32_t A;
    std::uint32_t B;
    std::uint32_t C;
    std::uint32_t D;

    void printValuesSum(){
        std::cout << "Valor: " << A+B+C+D << std::endl;
    }
};


//using Alloc_Pool = RandysEngine::SlotMap::SlotMap<Estructura,RandysEngine::Pool::Static_pool_allocator<Estructura,0>>;

int main(){

    RandysEngine::SlotMap::SlotMap<Estructura> prueba = RandysEngine::SlotMap::SlotMap<Estructura>(100);
  
    return 0;
}