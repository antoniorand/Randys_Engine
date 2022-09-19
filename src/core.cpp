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
    struct interact_visitor{
        template< typename T >
        void operator() ( T& value ){
            value.interact();
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
        
        {
            auto iteratorR = layers.rbegin();
            auto itEndR = layers.rend();

            while(iteratorR != itEndR){
                std::visit( interact_visitor{}, *iteratorR);
                iteratorR++;
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

}