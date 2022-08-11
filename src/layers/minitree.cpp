#include "minitree.hpp"


namespace RandysEngine{

    bool layer_minitree::addModel(){
        bool devolver = true;

        if(models.current_size() != models.max_capacity() && nodes.size()!= nodes.capacity()){
            RandysEngine::MinitreeNode node;
            
        }
        else
            devolver = false;

        return devolver;
    }

}