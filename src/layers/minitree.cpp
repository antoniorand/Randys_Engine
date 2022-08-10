#include "minitree.hpp"


namespace RandysEngine{

    bool layer_minitree::addModel(){
        bool devolver = true;

        if(models.size() != models.capacity() && nodes.size()!= models.capacity()){
            RandysEngine::MinitreeNode node;
            
        }
        else
            devolver = false;

        return devolver;
    }

}