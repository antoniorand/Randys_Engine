#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

//////
#include <memoryPool.hpp>


int main(){ 
    {
        //auto poolilla = RandysEngine::Pool::Bucket {100,1000};
        //auto ptr1 = poolilla.allocate(100);
        //auto ptr2 = poolilla.allocate(100);
        //auto ptr3 = poolilla.allocate(100);
        //poolilla.deallocate(ptr2,100);
        //auto ptr4 = poolilla.allocate(100);
    }
    _CrtDumpMemoryLeaks();
    return 0;
}