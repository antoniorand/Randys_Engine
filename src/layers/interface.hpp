#pragma once

#include "../resourceManager/resourceManager.hpp"

namespace RandysEngine{

    struct Layer_Element{
        RandysEngine::SlotMap::SlotMap_Key element;
        unsigned int layerId;
    };

    template<typename layer_type>
    class layer_interface{
        protected:
            ResourceManager& resource_manager;
            static inline unsigned int nextInstance{0};
            const unsigned int instance{nextInstance++};
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