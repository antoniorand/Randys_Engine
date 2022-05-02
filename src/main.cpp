
//////

/////
#include "memoryPool/memoryPool.hpp"
#include "memoryPool/memoryPool.hpp"
#include "resourceManager/resourceManager.hpp"
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

struct Data3{
    static inline int a {23};
    int x{a++}, y{a++}, z{a++};
};

int main(){

    //RandysEngine::SlotMap::SlotMap<Data,RandysEngine::Pool::Static_pool_allocator<Data,129>> slotmap1{128},slotmap2{128};

    RandysEngine::ResourceManager man;

    for(unsigned int i = 0; i < 1000;i++){
        Data ejemplo;
        Data2 ejemplo2;
        Data3 ejemplo3;
        const auto output = man.reserveResource<Data>(ejemplo);
        const auto output2 = man.reserveResource<Data2>(ejemplo2);
        const auto output3 = man.reserveResource<Data3>(ejemplo3);
        if(i == 21){
            auto& resource = *man.getResource<Data>(output);
            std::cout << "X: " << resource.x << " Y: " << resource.y << " Z: " << resource.z << std::endl;
        }
        if(i == 23){
            if(man.freeResource<Data2>(output2)){
                std::cout << "LIBERADOOOO\n";
            }
        }
    }

    

    return 0;
}