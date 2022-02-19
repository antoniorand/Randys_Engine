
//////

/////
#include <memoryPool/memoryPool.hpp>
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

using VectorPruebas = std::vector<Estructura,RandysEngine::Pool::Static_pool_allocator<Estructura,0>>;

int main(){


    std::cout << "Tam de struct: " << sizeof(Estructura) << std::endl;

    auto pruebas = VectorPruebas();

    pruebas.reserve(10);

    pruebas.push_back({2,2,2,2});
    pruebas.push_back({3,3,3,3});
    pruebas.push_back({4,4,4,4});
    pruebas.push_back({5,5,5,5});

    for(unsigned int i = 0; i < pruebas.size();i++){
        pruebas.at(i).printValuesSum();
    }

    return 0;
}