#include "core.hpp"

namespace RandysEngine{

    struct draw_visitor{

        template< typename T >
        void operator() ( T& value){
            value.draw();
        }   
    };
    struct interact_visitor{
        template< typename T >
        void operator() ( T& value ){
            value.interact();
        }   
    };

    void Rendering_Engine::runFrame(){

        /*
        
            In summary, this function will draw first and then
            it will check if it interacts with the inputs

        */
        {
            auto iterator = layers.begin();
            auto itEnd = layers.end();

            while(iterator != itEnd){
                std::visit( draw_visitor{}, *iterator);
                iterator++;
            }
        }
        
        {
            auto iteratorR = layers.rbegin();
            auto itEndR = layers.rend();

            while(iteratorR != itEndR){
                std::visit( interact_visitor{}, *iteratorR);
                iteratorR++;
            }
        }

    }

}