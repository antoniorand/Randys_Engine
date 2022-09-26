#include "core.hpp"

namespace RandysEngine{

    struct draw_visitor{

#ifndef __3DS__
                RandysEngine::gl_shader* shader;
#else
                RandysEngine::citro_shader* shader;
#endif

        draw_visitor(
#ifndef __3DS__
                RandysEngine::gl_shader& e_shader
#else
                RandysEngine::citro_shader& e_shader
#endif
        ){
            shader = &e_shader;
        };

        template< typename T >
        void operator() ( T& value){
            value.draw(shader);
        }   
    };

    void Rendering_Engine::runFrame(){

        /*
        
            In summary, this function will draw first and then
            it will check if it interacts with the mouse Input

        */

       screen.prepareDraw();
       shader.useShader();

        {
            auto iterator = layers.begin();
            auto itEnd = layers.end();

            while(iterator != itEnd){
                std::visit( draw_visitor{this->shader}, *iterator);
                iterator++;
            }
        }
        
        screen.swapBuffers();

    }

    bool Rendering_Engine::meshExists(std::string file){

        bool devolver = false;

        auto search = resources.find(file);

        if(search != resources.end())
            devolver = true;

        return devolver;

    }

    bool Rendering_Engine::reserveMeshResource(std::string file){

        bool devolver = false;

        if(!meshExists(file)){
#ifndef __3DS__
            auto key = ResourceManager.createResource<gl_mesh_resource>(file);
#else
            auto key = ResourceManager.createResource<citro_mesh_resource>(file);
#endif
            resources.emplace(std::make_pair(file, key));
            devolver = true;
        }

        return devolver;

    }

    bool Rendering_Engine::textureExists(std::string file){

        bool devolver = false;

        auto search = resources.find(file);

        if(search != resources.end())
            devolver = true;

        return devolver;

    }

    bool Rendering_Engine::reserveTextureResource(std::string file){

        bool devolver = false;

        if(!textureExists(file)){
#ifndef __3DS__
            auto key = ResourceManager.createResource<gl_mesh_resource>(file);
#else
            auto key = ResourceManager.createResource<citro_mesh_resource>(file);
#endif
            resources.emplace(std::make_pair(file, key));
            devolver = true;
        }

        return devolver;

    }

    struct Visitor_setTranslationMatrix{
    
        RandysEngine::Layer_Node node;
        float x;
        float y;
        float z;

        Visitor_setTranslationMatrix(RandysEngine::Layer_Node e_node, float e_x, float e_y, float e_z)
            :node{e_node},x{e_x},y{e_y},z{e_z}{}

        template< typename T >
        bool operator() ( T& value){
            return value.setTranslationMatrix(node,x,y,z);
        }  
    };

    void Rendering_Engine::setTranslateNode(RandysEngine::Layer_Node node, float x, float y, float z) noexcept{
        if(node.isValid){
            auto iterator = layers.begin();
            auto itEnd = layers.end();

            while(iterator != itEnd){
                bool result = std::visit(Visitor_setTranslationMatrix{node,x,y,z},*iterator);
                if(result)
                    break;
            }
        }
    }

    struct Visitor_setScalationMatrix{
    
        RandysEngine::Layer_Node node;
        float x;
        float y;
        float z;

        Visitor_setScalationMatrix(RandysEngine::Layer_Node e_node, float e_x, float e_y, float e_z)
            :node{e_node},x{e_x},y{e_y},z{e_z}{}

        template< typename T >
        bool operator() ( T& value){
            return value.setScalationMatrix(node,x,y,z);
        }  
    };

    void Rendering_Engine::setScaleNode(RandysEngine::Layer_Node node, float x, float y, float z) noexcept{
        if(node.isValid){
            auto iterator = layers.begin();
            auto itEnd = layers.end();

            while(iterator != itEnd){
                bool result = std::visit(Visitor_setScalationMatrix{node,x,y,z},*iterator);
                if(result)
                    break;
            }
        }
    }

    struct Visitor_setRotationMatrix{
    
        RandysEngine::Layer_Node node;
        float x;
        float y;
        float z;

        Visitor_setRotationMatrix(RandysEngine::Layer_Node e_node, float e_x, float e_y, float e_z)
            :node{e_node},x{e_x},y{e_y},z{e_z}{}

        template< typename T >
        bool operator() ( T& value){
            return value.setRotationMatrix(node,x,y,z);
        }  
    };

    void Rendering_Engine::setRotateNode(RandysEngine::Layer_Node node, float x, float y, float z) noexcept{
        if(node.isValid){
            auto iterator = layers.begin();
            auto itEnd = layers.end();

            while(iterator != itEnd){
                bool result = std::visit(Visitor_setRotationMatrix{node,x,y,z},*iterator);
                if(result)
                    break;
            }
        }
    }

    struct Visitor_TranslateMatrix{
    
        RandysEngine::Layer_Node node;
        float x;
        float y;
        float z;

        Visitor_TranslateMatrix(RandysEngine::Layer_Node e_node, float e_x, float e_y, float e_z)
            :node{e_node},x{e_x},y{e_y},z{e_z}{}

        template< typename T >
        bool operator() ( T& value){
            return value.TranslateMatrix(node,x,y,z);
        }  
    };

    void Rendering_Engine::TranslateNode(RandysEngine::Layer_Node node, float x, float y, float z) noexcept{
        if(node.isValid){
            auto iterator = layers.begin();
            auto itEnd = layers.end();

            while(iterator != itEnd){
                bool result = std::visit(Visitor_TranslateMatrix{node,x,y,z},*iterator);
                if(result)
                    break;
            }
        }
    }

    struct Visitor_ScaleMatrix{
    
        RandysEngine::Layer_Node node;
        float x;
        float y;
        float z;

        Visitor_ScaleMatrix(RandysEngine::Layer_Node e_node, float e_x, float e_y, float e_z)
            :node{e_node},x{e_x},y{e_y},z{e_z}{}

        template< typename T >
        bool operator() ( T& value){
            return value.ScaleMatrix(node,x,y,z);
        }  
    };

    void Rendering_Engine::ScaleNode(RandysEngine::Layer_Node node, float x, float y, float z) noexcept{
        if(node.isValid){
            auto iterator = layers.begin();
            auto itEnd = layers.end();

            while(iterator != itEnd){
                bool result = std::visit(Visitor_ScaleMatrix{node,x,y,z},*iterator);
                if(result)
                    break;
            }
        }
    }
    
    struct Visitor_RotateMatrix{
    
        RandysEngine::Layer_Node node;
        float x;
        float y;
        float z;

        Visitor_RotateMatrix(RandysEngine::Layer_Node e_node, float e_x, float e_y, float e_z)
            :node{e_node},x{e_x},y{e_y},z{e_z}{}

        template< typename T >
        bool operator() ( T& value){
            return value.RotateMatrix(node,x,y,z);
        }  
    };

    void Rendering_Engine::RotateNode(RandysEngine::Layer_Node node, float x, float y, float z) noexcept{
        if(node.isValid){
            auto iterator = layers.begin();
            auto itEnd = layers.end();

            while(iterator != itEnd){
                bool result = std::visit(Visitor_RotateMatrix{node,x,y,z},*iterator);
                if(result)
                    break;
            }
        }
    }

    struct Visitor_getInstanceLayer{

        template <typename T>
        const unsigned int operator()(T& value){
            return value.getInstance();
        }
    };

    struct Visitor_getTranslationMatrix{
    
        RandysEngine::Layer_Node node;

        Visitor_getTranslationMatrix(RandysEngine::Layer_Node e_node)
            :node{e_node}{}

        template< typename T >
        std::array<float,3> operator() ( T& value){
            return value.getTranslationMatrix(node);
        }  
    };

    std::array<float,3> Rendering_Engine::getTranslationNode(RandysEngine::Layer_Node node) noexcept{
        std::array<float,3> devolver;
        if(node.isValid){
            auto iterator = layers.begin();
            auto itEnd = layers.end();

            while(iterator != itEnd){
                const unsigned int layerInstance = std::visit(Visitor_getInstanceLayer{},*iterator);
                if(node.layerId == layerInstance){
                    devolver = std::visit(Visitor_getTranslationMatrix{node},*iterator);
                    break;
                }
            }
        }
        return devolver;
    }

    struct Visitor_getRotationMatrix{
    
        RandysEngine::Layer_Node node;

        Visitor_getRotationMatrix(RandysEngine::Layer_Node e_node)
            :node{e_node}{}

        template< typename T >
        std::array<float,3> operator() ( T& value){
            return value.getRotationMatrix(node);
        }  
    };

    std::array<float,3> Rendering_Engine::getRotationNode(RandysEngine::Layer_Node node) noexcept{
        std::array<float,3> devolver;
        if(node.isValid){
            auto iterator = layers.begin();
            auto itEnd = layers.end();

            while(iterator != itEnd){
                const unsigned int layerInstance = std::visit(Visitor_getInstanceLayer{},*iterator);
                if(node.layerId == layerInstance){
                    devolver = std::visit(Visitor_getRotationMatrix{node},*iterator);
                    break;
                }
            }
        }
        return devolver;
    }

    struct Visitor_getScalationMatrix{
    
        RandysEngine::Layer_Node node;

        Visitor_getScalationMatrix(RandysEngine::Layer_Node e_node)
            :node{e_node}{}

        template< typename T >
        std::array<float,3> operator() ( T& value){
            return value.getScalationMatrix(node);
        }  
    };

    std::array<float,3> Rendering_Engine::getScalationNode(RandysEngine::Layer_Node node) noexcept{
        std::array<float,3> devolver;
        if(node.isValid){
            auto iterator = layers.begin();
            auto itEnd = layers.end();

            while(iterator != itEnd){
                const unsigned int layerInstance = std::visit(Visitor_getInstanceLayer{},*iterator);
                if(node.layerId == layerInstance){
                    devolver = std::visit(Visitor_getScalationMatrix{node},*iterator);
                    break;
                }
            }
        }
        return devolver;
    }

}