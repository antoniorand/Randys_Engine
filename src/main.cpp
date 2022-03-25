
//////
#include <glad/glad.h>
#include <GLFW/glfw3.h>
/////
#include <memoryPool/memoryPool.hpp>
#include <memoryPool/user_defined_descriptor.hpp>
#include <slotmap/slotmap.hpp>
#include <vector>
#include <iostream>

struct Estructura{
    int x, y;
};

struct Estructura2{
    int x,y,z,w;
    uint32_t a,b,c,d;
};

using DATA_ALLOC = RandysEngine::Pool::Static_pool_allocator<Estructura,10>;
using SlotmapEstandar = RandysEngine::SlotMap<Estructura>;
using SlotmapMemoryPool = RandysEngine::SlotMap<Estructura, DATA_ALLOC>;
using SlotmapMemoryPool2 = RandysEngine::SlotMap<Estructura2, DATA_ALLOC>;

int main(){

    SlotmapMemoryPool slotmap = SlotmapMemoryPool(5000);

    std::cout << sizeof(SlotmapMemoryPool) << " " << sizeof(SlotmapMemoryPool2) << std::endl;
    
    /*glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}