#pragma once

#ifndef __3DS__
    #include "../APIs/generic/cameraOpengl.hpp"
#endif

    #include "../resourceManager/resourceManager.hpp"

namespace RandysEngine{

    class layer_interface{
        public:
            layer_interface(){};
            virtual ~layer_interface() {};
            virtual void activate() = 0;
            virtual void deactivate() = 0;
            virtual bool draw(Camera& activeCamera, ResourceManager& man) = 0;
            virtual bool interact() = 0;
    };

}