#include "core.hpp"

namespace RandysEngine{

    struct draw_visitor{

        ResourceManager& manager;

        draw_visitor(ResourceManager& m) : manager{m}{}

        template< typename T >
        void operator() ( T& value){
            value.draw(manager);
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
                std::visit( draw_visitor{ResourceManager}, *iterator);
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