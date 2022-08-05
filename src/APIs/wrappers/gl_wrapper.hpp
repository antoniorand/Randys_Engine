#pragma once
#include "base_wrapper.hpp"
#include <GLFW/glfw3.h>
#include "../GLAD/glad.h"
#include <iostream>

struct gl_screen : screen_wrapper<gl_screen>{

    GLFWwindow* window;

    unsigned int width {400},height{240};

    gl_screen(){

    }
    ~gl_screen(){
    }

    void changeWindowSize(unsigned int e_width, unsigned int e_height){
    }

};

struct gl_initializer : initializer_wrapper<gl_initializer>{

    gl_initializer(){
    };
    ~gl_initializer(){
    };


};