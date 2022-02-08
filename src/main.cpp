
#include <memoryPool.hpp>
#include <iostream>

struct Ejemplo{

    unsigned int hola{1};
    int hey{1};

    Ejemplo() = default;

};


int main(){ 

    auto& poolilla = RandysEngine::Pool::get_instance<1>();

    std::cout << poolilla.size() << std::endl;
    std::cout << poolilla.at(0).blockSize << " " << poolilla.at(0).blockCount << std::endl;
    std::cout << poolilla.at(1).blockSize << " " << poolilla.at(1).blockCount << std::endl;
    std::cout << poolilla.at(2).blockSize << " " << poolilla.at(2).blockCount << std::endl;

    return 0;
}