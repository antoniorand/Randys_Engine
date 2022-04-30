
//////

/////
#include <memoryPool/memoryPool.hpp>
#include <slotmap/slotmap.hpp>
#include <resourceManager/resourceManager.hpp>
#include <vector>
#include <iostream>
#include <memory>

struct Data{
    static inline int a {0};

    int x{a++}, y{a++}, z{a++};
};

struct Data2{
    static inline int a {11};

    int x{a++}, y{a++}, z{a++};
};

int main(){

    //RandysEngine::SlotMap::SlotMap<Data,RandysEngine::Pool::Static_pool_allocator<Data,129>> slotmap1{128},slotmap2{128};

    RandysEngine::ResourceManager man;

    for(unsigned int i = 0; i < 5000;i++){
        Data ejemplo;
        Data2 ejemplo2;
        const auto output = man.reserveResource<Data>(ejemplo);
        const auto output2 = man.reserveResource<Data2>(ejemplo2);
        std::cout << "Llave: 1) " << output.first << " 2) " << output.second << std::endl;
        std::cout << "Llave: 1) " << output2.first << " 2) " << output2.second << std::endl;

    }
    

    return 0;
}