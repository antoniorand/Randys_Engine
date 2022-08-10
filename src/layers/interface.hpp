#pragma once

#ifndef __3DS__
#endif

    #include "../resourceManager/resourceManager.hpp"

namespace RandysEngine{

    template<typename layer_type>
    class layer_interface{
        public:
            layer_interface(ResourceManager& man){};
            virtual ~layer_interface() {};
            void activate(){
                static_cast<layer_type*>(this)->activate();
            }
            void deactivate(){
                static_cast<layer_type*>(this)->deactivate();
            }
            bool draw(ResourceManager& man){
                return static_cast<layer_type*>(this)->draw();
            }
            bool interact(){
                return static_cast<layer_type*>(this)->interact();
            }
    };

}