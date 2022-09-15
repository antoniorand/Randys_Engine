#include "core.hpp"

namespace RandysEngine{

    struct draw_visitor{

        template< typename T >
        void operator() ( T& value){
            value.draw();
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
                std::visit( draw_visitor{}, *iterator);
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


}