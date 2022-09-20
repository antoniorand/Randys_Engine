#pragma once

#include "../resourceManager/resourceManager.hpp"
#include "../resourceManager/entities.hpp"

#ifndef __3DS__
    #include <glm/glm.hpp>
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

    template<typename layer_type>
    class layer_interface{
        protected:
            ResourceManager& resource_manager;

            static inline unsigned int nextInstance{0};
            const unsigned int instance{nextInstance++};

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
            bool interact() const{
                return static_cast<layer_type*>(this)->interact();
            }

            const unsigned int getInstance(){return instance;};

            [[nodiscard]] const RandysEngine::Layer_Node createNode() noexcept{
                return static_cast<layer_type*>(this)->createNode();
            }

            [[nodiscard]] const RandysEngine::Layer_Node createNode(const RandysEngine::Layer_Node node) noexcept{
                return static_cast<layer_type*>(this)->createNode(node);
            }

            void addModel(const RandysEngine::Layer_Node node) noexcept{
                static_cast<layer_type*>(this)->addModel(node);
            }

            RandysEngine::Model_Entity* 
                getModel(const RandysEngine::Layer_Node node) const noexcept{
                return static_cast<layer_type*>(this)->getModel(node);
            }

            bool setTranslationMatrix(const RandysEngine::Layer_Node node, float x, float y, float z) const noexcept{
                return static_cast<layer_type*>(this)->setTranslationMatrix(node,x,y,z);
            }
            bool setScalationMatrix(const RandysEngine::Layer_Node node, float x, float y, float z) const noexcept{
                return static_cast<layer_type*>(this)->setScalationMatrix(node,x,y,z);
            }
            bool setRotationMatrix(const RandysEngine::Layer_Node node, float x, float y, float z) const noexcept{
                return static_cast<layer_type*>(this)->setRotationMatrix(node,x,y,z);
            }

            bool TranslateMatrix(const RandysEngine::Layer_Node node, float x, float y, float z) const noexcept{
                return static_cast<layer_type*>(this)->TranslateMatrix(node,x,y,z);
            }
            bool ScaleMatrix(const RandysEngine::Layer_Node node, float x, float y, float z) const noexcept{
                return static_cast<layer_type*>(this)->ScaleMatrix(node,x,y,z);
            }
            bool RotateMatrix(const RandysEngine::Layer_Node node, float x, float y, float z) const noexcept{
                return static_cast<layer_type*>(this)->RotateMatrix(node,x,y,z);
            }

            std::array<float,3> getTranslationMatrix(const RandysEngine::Layer_Node node) const noexcept{
                return static_cast<layer_type*>(this)->getTranslationMatrix(node);
            }
            std::array<float,3> getScalationMatrix(const RandysEngine::Layer_Node node) const noexcept{
                return static_cast<layer_type*>(this)->getScalationMatrix(node);
            }
            std::array<float,3> getRotationMatrix(const RandysEngine::Layer_Node node) const noexcept{
                return static_cast<layer_type*>(this)->getRotationMatrix(node);
            }

    };

}