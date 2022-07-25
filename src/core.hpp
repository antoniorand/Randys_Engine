#include "memoryPool/memoryPool.hpp"
#include "memoryPool/memoryPool.hpp"
#include "resourceManager/resourceManager.hpp"
#include "layers/layers.hpp"
#include <iostream>
#include <variant>
#include <list>
#ifndef __3DS__
    #include "APIs/GLAD/glad.h"
    #include <GLFW/glfw3.h>
#endif

namespace RandysEngine{

    //The current layer types we are going to run
    using layerTypes = std::variant<
            RandysEngine::layer_skybox,
            RandysEngine::layer_GUI
            >;
    //the maximum number of layers
    constexpr std::size_t maxLayers = 10;

    //the type of layer list, it has a maximum capacity
    using LayerList = std::list<
            layerTypes, RandysEngine::Pool::Static_pool_allocator<layerTypes,maxLayers+1>
        >;
    
    //The rendering Engine by itself
    class Rendering_Engine{
        
        //The Resource manager that stores data on the heap
        //with optimizations
        RandysEngine::ResourceManager ResourceManager;

        //The list of layers
        LayerList layers;

        public:
            
            Rendering_Engine(){};
            ~Rendering_Engine(){};

            //Templated function that lets you add a layer of an specific type in the back of the list
            template<typename Layer_Type,typename... Args>
            bool addLayerBack(Args... args){
                bool devolver = false;

                if(layers.size() < maxLayers){
                    Layer_Type newLayer{args...};
                    devolver = true;
                    //std::cout << "Inserting layer of type " << typeid(Layer_Type).name() << " in the back of the list\n"; 
                    layers.push_back(newLayer);
                }
                        
                return devolver;
            }

            //Templated function that lets you add a layer of an specific type in an specific position
            template<typename Layer_Type,typename... Args>
            bool addLayer(std::size_t position,Args... args){
                bool devolver = false;

                if(layers.size() != layers.max_size() && position <= layers.size()){
                    Layer_Type newLayer{args...};
                    auto itBegin = layers.begin();
                    for(unsigned int i = 0; i < position;i++)
                        itBegin++;
                    devolver = true;
                    //std::cout << "Inserting layer of type " << typeid(Layer_Type).name() << " in the position " << position << std::endl;
                    layers.insert(itBegin,newLayer);
                }
                        
                return devolver;
            }
            //Delete the layer number something of an specific layer.
            //Example, if you have three GUI layers, and input 1, you will delete the third GUI layer
            template<typename Layer_type>
            bool deleteLayer(std::size_t index_of_layer){
                bool devolver = false;
                std::size_t counter = 0;
                
                auto itBegin = layers.begin();
                auto itEnd = layers.end();
                while(itBegin != itEnd){
                    if(std::holds_alternative<Layer_type>(*itBegin)){
                        if(counter == index_of_layer){
                            layers.erase(itBegin);
                            devolver = true;
                            break;
                        }
                        else counter++;
                    }
                    itBegin++;
                }                
                return devolver;
            }

            //Run the frame, not only drawing it but interacting with it
            void runFrame();
    };

};