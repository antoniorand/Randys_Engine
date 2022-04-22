
//////

/////
#include <memoryPool/memoryPool.hpp>
#include <slotmap/slotmap.hpp>
//#include <resourceManager/resourceManager.hpp>
#include <vector>
#include <iostream>
#include <memory>

struct Data{
    static inline int a {0};

    int x{a++}, y{a++}, z{a++};
};

using AllocData = RandysEngine::Pool::Static_pool_allocator<Data,2000>;
using Slotmap = RandysEngine::SlotMap<Data,AllocData>;

int main(){

    Slotmap map{1000};
    
    

    return 0;
}