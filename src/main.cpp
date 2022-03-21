
//////
#include <glad/glad.h>
#include <GLFW/glfw3.h>
/////
#include <memoryPool/memoryPool.hpp>
#include <slotmap/slotmap.hpp>
#include <vector>
#include <iostream>


//using Alloc_Pool = RandysEngine::SlotMap::SlotMap<Estructura,RandysEngine::Pool::Static_pool_allocator<Estructura,0>>;

int main(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}