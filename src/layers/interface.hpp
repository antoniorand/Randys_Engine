#pragma once

#ifndef __3DS__
#endif

    #include "../resourceManager/resourceManager.hpp"

namespace RandysEngine{

    template<typename layer_type>
    class layer_interface{
        ResourceManager& resource_manager;
        public:
            layer_interface(ResourceManager& man) : resource_manager{man}{};
            virtual ~layer_interface() {};
            void activate(){
                static_cast<layer_type*>(this)->activate();
            }
            void deactivate(){
                static_cast<layer_type*>(this)->deactivate();
            }
            bool draw() const {
                return static_cast<layer_type*>(this)->draw();
            }
            bool interact() const{
                return static_cast<layer_type*>(this)->interact();
            }
    };

}