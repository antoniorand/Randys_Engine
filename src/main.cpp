
//////

/////
#include <memoryPool/memoryPool.hpp>
#include <slotmap/slotmap.hpp>
#include <vector>
#include <iostream>

struct Estructura{

    int A;
    int B;
};

//using Alloc_Pool = RandysEngine::SlotMap::SlotMap<Estructura,RandysEngine::Pool::Static_pool_allocator<Estructura,0>>;

int main(){

    RandysEngine::SlotMap::SlotMap<Estructura,8> prueba = RandysEngine::SlotMap::SlotMap<Estructura,8>();

    [[maybe_unused]]auto slot1 = prueba.push_back({1,2});
    [[maybe_unused]]auto slot2 = prueba.push_back(2,3);
    [[maybe_unused]]auto slot3 = prueba.push_back(2,3);
    [[maybe_unused]]auto slot4 = prueba.push_back(2,3);

    std::cout << "Slot1: id " << slot1.Id <<", gen " << slot1.Gen << std::endl;
    std::cout << "Slot2: id " << slot2.Id <<", gen " << slot2.Gen << std::endl;

    std::cout << "Free List Index: " << prueba.getFreeList() << "\n";
    prueba.printIndices();
    std::cout << std::endl;
    std::cout << "Data\n";
    std::cout << "Data[0]:" << (prueba.atPosition(0)->A) << ", " << (prueba.atPosition(0)->B) << std::endl;
    std::cout << "Data[1]:" << (prueba.atPosition(1)->A) << ", " << (prueba.atPosition(1)->B) << std::endl;
    std::cout << "Data[1]:" << (prueba.atPosition(slot2)->A) << ", " << (prueba.atPosition(slot2)->B) << std::endl;


    prueba.erase(slot2);
    slot2 = prueba.push_back(5,6);

    std::cout << std::endl;
    std::cout << "Data\n";
    std::cout << "Data[0]:" << (prueba.atPosition(0)->A) << ", " << (prueba.atPosition(0)->B) << std::endl;
    std::cout << "Data[1]:" << (prueba.atPosition(1)->A) << ", " << (prueba.atPosition(1)->B) << std::endl;
    std::cout << "Data[2]:" << (prueba.atPosition(2)->A) << ", " << (prueba.atPosition(2)->B) << std::endl;
    std::cout << "Data[3]:" << (prueba.atPosition(slot2)->A) << ", " << (prueba.atPosition(slot2)->B) << std::endl;


    return 0;
}