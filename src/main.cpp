
//////
#include <glad/glad.h>
#include <GLFW/glfw3.h>
/////
#include <memoryPool/memoryPool.hpp>
#include <memoryPool/user_defined_descriptor.hpp>
#include <slotmap/slotmap.hpp>
#include <resourceManager/resourceManager.hpp>
#include <vector>
#include <iostream>


int main(){

    RandysEngine::ResourceManager gestor{};

    gestor.addResource<RandysEngine::Model>();
    gestor.addResource<RandysEngine::Mesh>();
    gestor.addResource<RandysEngine::Vertex>();

    /*glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}