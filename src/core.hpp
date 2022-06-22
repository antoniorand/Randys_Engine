#include "memoryPool/memoryPool.hpp"
#include "memoryPool/memoryPool.hpp"
#include "resourceManager/resourceManager.hpp"
#include "layers/layers.hpp"
#include <iostream>
#include <variant>
#include <list>

namespace RandysEngine{

    using layerTypes = std::variant<
            RandysEngine::layer_skybox,
            RandysEngine::layer_GUI
            >;

    constexpr std::size_t maxLayers = 10;

    using LayerList = std::list<
            layerTypes, RandysEngine::Pool::Static_pool_allocator<layerTypes,maxLayers+1>
        >;
    
    class Rendering_Engine{
        
        RandysEngine::ResourceManager ResourceManager;

        LayerList layers;

        public:
            Rendering_Engine(){
            };
            ~Rendering_Engine(){};


            /*template<typename Layer_type, typename... Arguments>
            bool addLayerBack(Arguments... args){
                bool devolver = false;

                if(layers.size() != layers.capacity()){
                    Layer_type newLayer{args...};
                    devolver = true;
                    layers.push_back(newLayer);
                }
                        
                return devolver;
            }*/

            template<typename Layer_Type,typename... Args>
            bool addLayerBack(Args... args){
                bool devolver = false;

                if(layers.size() != layers.max_size()){
                    Layer_Type newLayer{args...};
                    devolver = true;
                    layers.push_back(newLayer);
                }
                        
                return devolver;
            }

            template<typename Layer_Type,typename... Args>
            bool addLayer(std::size_t position,Args... args){
                bool devolver = false;

                if(layers.size() != layers.max_size() && position <= layers.size()){
                    Layer_Type newLayer{args...};
                    auto itBegin = layers.begin();
                    for(unsigned int i = 0; i < position;i++)
                        itBegin++;
                    devolver = true;
                    layers.insert(itBegin,newLayer);
                }
                        
                return devolver;
            }

            /*template<typename Layer_type, typename... Arguments>
            bool addLayer(std::size_t position,Arguments... args){
                bool devolver = false;

                if(layers.size() != layers.capacity() && position < layers.capacity()){
                    Layer_type newLayer{args...};
                    devolver = true;
                    layers.insert(layers.begin()+position,newLayer);
                }
                        
                return devolver;
            }*/

            /*template<typename Layer_type>
            bool deleteLayer(std::size_t index_of_layer){
                bool devolver = false;
                std::size_t counter = 0;
                for(std::size_t i = 0; i < layers.size();i++){
                    if(std::holds_alternative<Layer_type>(layers[i])){
                        if(counter == index_of_layer){
                            layers.erase(layers.begin()+i);
                            devolver = true;
                            break;
                        }
                        else counter++;
                    }
                }
                
                return devolver;
            }*/
    };

};