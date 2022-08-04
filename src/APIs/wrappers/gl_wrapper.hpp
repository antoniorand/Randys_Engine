#pragma once
#include "base_wrapper.hpp"
#include <GLFW/glfw3.h>
#include "../GLAD/glad.h"
#include <iostream>

struct gl_screen : screen_wrapper<gl_screen>{

    GLFWwindow* window;

    unsigned int width {400},height{240};

    gl_screen(){

        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(width,height,"RandysEngine Example Text",nullptr,nullptr);

        if (window == NULL){
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            //throw err or something
        }
    
        glfwMakeContextCurrent(window);
        //glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        //glfwSetCursorPosCallback(window, mouse_callback);
        //glfwSetScrollCallback(window, scroll_callback);

        // tell GLFW to capture our mouse
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    }
    ~gl_screen(){
        glfwTerminate();
    }

    void changeWindowSize(unsigned int e_width, unsigned int e_height){
        width = e_width;
        height = e_height;
    }

};

struct gl_initializer : initializer_wrapper<gl_initializer>{

    gl_initializer(){

        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
            std::cout << "Failed to initialize GLAD" << std::endl;
        }  

        glEnable(GL_DEPTH_TEST);
    };
    ~gl_initializer(){

        

    };


};