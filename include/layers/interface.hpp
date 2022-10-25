#pragma once

#include "../resourceManager/resourceManager.hpp"
#include "../resourceManager/entities.hpp"

#ifndef __3DS__
    #include <dependencies/glm/glm.hpp>
    #include "../APIs/wrappers/gl_wrapper.hpp"
#else
    #include "../APIs/wrappers/citro_wrapper.hpp"
#endif

namespace RandysEngine{

    struct Layer_Node{
        RandysEngine::SlotMap::SlotMap_Key reference;
        unsigned int layerId;
        bool isValid{false};
    };

    class protectedData{

        protected:
            static inline unsigned int nextInstance{0};
            const unsigned int instance{nextInstance++};
            protectedData(){};
    };

    template<typename layer_type>
    class layer_interface : public protectedData{
        protected:
            ResourceManager& resource_manager;

            bool activated {true};
        public:

            layer_interface(ResourceManager& man) : resource_manager{man}{};
            virtual ~layer_interface() {};
            void activate(){
                static_cast<layer_type*>(this)->activate();
            }
            void deactivate(){
                static_cast<layer_type*>(this)->deactivate();
            }
            bool draw(
#ifndef __3DS__
                RandysEngine::gl_shader* shader
#else
                RandysEngine::citro_shader* shader
#endif
            ){
                return static_cast<layer_type*>(this)->draw();
            }

            const unsigned int getInstance(){return instance;};

            [[nodiscard]] const RandysEngine::Layer_Node createNode() noexcept{
                return static_cast<layer_type*>(this)->createNode();
            }

            [[nodiscard]] const RandysEngine::Layer_Node createNode(RandysEngine::Layer_Node& node) noexcept{
                return static_cast<layer_type*>(this)->createNode(node);
            }

            bool setTranslationMatrix(RandysEngine::Layer_Node& node, float x, float y, float z) const noexcept{
                return static_cast<layer_type*>(this)->setTranslationMatrix(node,x,y,z);
            }
            bool setScalationMatrix(RandysEngine::Layer_Node& node, float x, float y, float z) const noexcept{
                return static_cast<layer_type*>(this)->setScalationMatrix(node,x,y,z);
            }
            bool setRotationMatrix(RandysEngine::Layer_Node& node, float x, float y, float z) const noexcept{
                return static_cast<layer_type*>(this)->setRotationMatrix(node,x,y,z);
            }

            bool TranslateMatrix(RandysEngine::Layer_Node& node, float x, float y, float z) const noexcept{
                return static_cast<layer_type*>(this)->TranslateMatrix(node,x,y,z);
            }
            bool ScaleMatrix(RandysEngine::Layer_Node& node, float x, float y, float z) const noexcept{
                return static_cast<layer_type*>(this)->ScaleMatrix(node,x,y,z);
            }
            bool RotateMatrix(RandysEngine::Layer_Node& node, float x, float y, float z) const noexcept{
                return static_cast<layer_type*>(this)->RotateMatrix(node,x,y,z);
            }

            std::array<float,3> getTranslationMatrix(RandysEngine::Layer_Node& node) const noexcept{
                return static_cast<layer_type*>(this)->getTranslationMatrix(node);
            }
            std::array<float,3> getScalationMatrix(RandysEngine::Layer_Node& node) const noexcept{
                return static_cast<layer_type*>(this)->getScalationMatrix(node);
            }
            std::array<float,3> getRotationMatrix(RandysEngine::Layer_Node& node) const noexcept{
                return static_cast<layer_type*>(this)->getRotationMatrix(node);
            }

    };

}