#pragma once


template<typename api>
struct initializer_wrapper {
    


};

template<typename api>
struct screen_wrapper{

    void changeWindowSize(unsigned int e_width, unsigned int e_height){
        api.template changeWindowSize(e_width,e_height);
    }

};