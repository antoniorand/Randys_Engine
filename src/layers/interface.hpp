#pragma once

namespace RandysEngine{

    class layer_interface{
        public:
            layer_interface(){};
            virtual ~layer_interface() {};
            virtual void activate() = 0;
            virtual void deactivate() = 0;
            virtual bool draw() = 0;
            virtual bool interact() = 0;
    };

}