
//////
#include <glad/glad.h>
#include <GLFW/glfw3.h>
/////
#include <memoryPool/memoryPool.hpp>
#include <slotmap/slotmap.hpp>
#include <vector>
#include <iostream>

struct Estructura{
    int x, y;
};

struct Estructura2{
    int x, y, z;
};
using DATA_ALLOC = RandysEngine::Pool::Static_pool_allocator<Estructura,0>;
using SlotmapEstandar = RandysEngine::SlotMap::SlotMap<Estructura>;
using SlotmapMemoryPool = RandysEngine::SlotMap::SlotMap<Estructura, DATA_ALLOC>;
using Key           = struct{std::uint32_t Id;std::uint64_t Gen;};
//Rebinding of the allocator provided
using INDICES_ALLOC = typename std::allocator_traits<DATA_ALLOC>::template rebind_alloc<Key>;
                
//Rebinding of the allocator provided to erase type
using ERASE_ALLOC   = typename std::allocator_traits<DATA_ALLOC>::template rebind_alloc<std::uint32_t>;

int main(){

    /*DATA_ALLOC allocatorData;
    INDICES_ALLOC allocatorIndices;
    ERASE_ALLOC allocatorErase;

    auto value = allocatorData.allocate(100);
    auto value2 = allocatorIndices.allocate(100);*/

    SlotmapMemoryPool slotmap;
    //auto key1 = slotmap.push_back({2,3});
    //auto valor = slotmap.atPosition(key1);
    //std::cout << "Valor x:" << valor->x << " \nValor y: " << valor->y << std::endl;
    /*glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}