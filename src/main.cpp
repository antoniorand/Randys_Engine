
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

using DATA_ALLOC = RandysEngine::Pool::Static_pool_allocator<Estructura,10>;
using SlotmapEstandar = RandysEngine::SlotMap::SlotMap<Estructura>;
using SlotmapMemoryPool = RandysEngine::SlotMap::SlotMap<Estructura, DATA_ALLOC>;

int main(){

    SlotmapMemoryPool slotmap = SlotmapMemoryPool(5000);

    auto key1 = slotmap.push_back({2,3});
    Estructura objeto{4,5};
    auto key2 = slotmap.push_back(objeto);
    auto valor = slotmap.atPosition(key1);
    auto valor2= slotmap.atPosition(key2);
    std::cout << "Valor x: " << valor->x << " \nValor y: " << valor->y << std::endl;
    std::cout << "Valor x: " << valor2->x << " \nValor y: " << valor2->y << std::endl;
    slotmap.erase(key1);
    slotmap.clear();
    
    /*glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}