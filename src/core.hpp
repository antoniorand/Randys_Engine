#pragma once
#include "memoryPool/memoryPool.hpp"
#include "resourceManager/resourceManager.hpp"

#ifndef __3DS__
    #include "APIs/wrappers/gl_wrapper.hpp"
#else
    #include "APIs/wrappers/citro_wrapper.hpp"
#endif

#include "layers/layers.hpp"
#include <iostream>
#include <variant>
#include <list>
#include <chrono>
#include <thread>


namespace RandysEngine{

    //The current layer types we are going to run
    using layerTypes = std::variant<
            RandysEngine::layer_skybox,
            RandysEngine::layer_GUI,
            RandysEngine::layer_minitree
        >;
    //the maximum number of layers
    constexpr std::size_t maxLayers = 10;

    //the type of layer list, it has a maximum capacity
    using LayerList = std::list<
            layerTypes, RandysEngine::Pool::Static_pool_allocator<layerTypes,maxLayers+1>
        >;
    
    //The rendering Engine by itself
    class Rendering_Engine{

#ifdef __3DS__
    //3ds API wrapper
    
        citro_main init{};
        citro_screen screen{};
        citro_shader shader{};

#else
    //opengl api wrapper

        gl_screen screen{};

        gl_main init{};

        gl_shader shader{};


#endif

        //The Resource manager that stores data on the heap
        //with optimizations
        RandysEngine::ResourceManager ResourceManager;

        //The list of layers
        LayerList layers;

        //Resources

        std::map<std::string,ResourceManager::KeyId> resources;

        static constexpr long long int frameTime{16*1000*1000};

        public:
            
            Rendering_Engine(){};
            ~Rendering_Engine(){};

            //Templated function that lets you add a layer of an specific type in the back of the list
            template<typename Layer_Type,typename... Args>
            bool addLayerBack(Args... args){
                bool devolver = false;

                if(layers.size() < maxLayers){
                    devolver = true;
                    //std::cout << "Inserting layer of type " << typeid(Layer_Type).name() << " in the back of the list\n"; 
                    layers.emplace_back(std::in_place_type<Layer_Type>,ResourceManager,args...);
                }
                        
                return devolver;
            }

            //Templated function that lets you add a layer of an specific type in an specific position
            template<typename Layer_Type,typename... Args>
            bool addLayer(std::size_t position,Args... args){
                bool devolver = false;

                if(layers.size() != layers.max_size() && position <= layers.size()){
                    auto itBegin = layers.begin();
                    for(unsigned int i = 0; i < position;i++)
                        itBegin++;
                    devolver = true;
                    //std::cout << "Inserting layer of type " << typeid(Layer_Type).name() << " in the position " << position << std::endl;
                    layers.emplace(itBegin,std::in_place_type<Layer_Type>,ResourceManager,args...);
                }
                        
                return devolver;
            }
            //Delete the layer number something of an specific layer.
            //Example, if you have three GUI layers, and input 1, you will delete the third GUI layer
            template<typename Layer_type>
            bool deleteLayer(std::size_t index_of_layer){
                bool devolver = false;
                
                auto itBegin = layers.begin();
                auto itEnd = layers.end();
                while(itBegin != itEnd){
                    if(std::holds_alternative<Layer_type>(*itBegin)){
                        Layer_type* layer = &std::get<Layer_type>(*itBegin);
                        if(layer->getInstance() == index_of_layer){
                            layers.erase(itBegin);
                            devolver = true;
                            break;
                        }
                    }
                    itBegin++;
                }                
                return devolver;
            }

            //Get the layer number something of an specific layer.
            //Example, if you have three GUI layers, and input 1, you will get the third GUI layer
            template<typename Layer_type>
            Layer_type* getLayer(std::size_t index_of_layer){
                Layer_type* devolver = nullptr;
                
                auto itBegin = layers.begin();
                auto itEnd = layers.end();
                while(itBegin != itEnd){
                    if(std::holds_alternative<Layer_type>(*itBegin)){
                        Layer_type* layer = &std::get<Layer_type>(*itBegin);
                        if(layer->getInstance() == index_of_layer){
                            devolver = &std::get<Layer_type>(*itBegin);
                            break;
                        }
                    }
                    itBegin++;
                }                
                return devolver;
            }

            //Create a node in a specific layer
            template<typename Layer_type>
            [[nodiscard]] Layer_Node createNode(std::size_t index_of_layer){
                Layer_Node devolver;
                
                auto itBegin = layers.begin();
                auto itEnd = layers.end();
                while(itBegin != itEnd){
                    if(std::holds_alternative<Layer_type>(*itBegin)){
                        Layer_type* layer = &std::get<Layer_type>(*itBegin);
                        if(layer->getInstance() == index_of_layer){
                            layer = &std::get<Layer_type>(*itBegin);

                            devolver = layer->createNode();
                            break;
                        }
                    }
                    itBegin++;
                }                
                return devolver;
            }

            //Create a node in a specific layer
            template<typename Layer_type>
            [[nodiscard]] Layer_Node createNode(RandysEngine::Layer_Node& parentNode){
                Layer_Node devolver;

                if(parentNode.isValid){
                  
                    auto itBegin = layers.begin();
                    auto itEnd = layers.end();

                    while(itBegin != itEnd){
                        if(std::holds_alternative<Layer_type>(*itBegin)){

                            Layer_type* layer = &std::get<Layer_type>(*itBegin);

                            if(layer->getInstance() == parentNode.layerId){
                            
                                devolver = layer->createNode(parentNode);
                                break;
                            
                            }
                        
                        }
                        itBegin++;
                    }         
                }       
                return devolver;
            }

            //Delete a node in a specific layer
            template<typename Layer_type>
            bool deleteNode(RandysEngine::Layer_Node& toDestroy){
                bool devolver = false;

                if(toDestroy.isValid){

                    auto itBegin = layers.begin();
                    auto itEnd = layers.end();

                    while(itBegin != itEnd){
                        if(std::holds_alternative<Layer_type>(*itBegin)){
                            Layer_type* layer = &std::get<Layer_type>(*itBegin);

                            if(layer->getInstance() == toDestroy.layerId){
                            
                                devolver = layer->deleteNode(toDestroy);
                                break;
                            
                            }
                        }
                        itBegin++;
                    }         

                }

                return devolver;
            }


            //Create a node in a specific layer
            template<typename Layer_type>
            void addModel(RandysEngine::Layer_Node& node){
                if(node.isValid){

                    auto itBegin = layers.begin();
                    auto itEnd = layers.end();
                    while(itBegin != itEnd){
                        if(std::holds_alternative<Layer_type>(*itBegin)){
                            Layer_type* layer = &std::get<Layer_type>(*itBegin);
                            if(node.layerId == layer->getInstance()){

                                layer->addModel(node);
                                break;
                            }
                        }
                        itBegin++;
                    }
                }
            }

            //Create a node in a specific layer
            template<typename Layer_type>
            void addCamera(RandysEngine::Layer_Node& node, 
                float fov = 45.0f, float height = 400.0f, float width = 240.0f, float near = 0.1f, float far = 1000.0f){
                
                if(node.isValid){

                    auto itBegin = layers.begin();
                    auto itEnd = layers.end();
                    while(itBegin != itEnd){
                        if(std::holds_alternative<Layer_type>(*itBegin)){
                            Layer_type* layer = &std::get<Layer_type>(*itBegin);
                            if(node.layerId == layer->getInstance()){

                                layer->addCamera(node,fov,height/width,near,far);
                                break;
                            }
                        }
                        itBegin++;
                    }
                }
            }


            //Create a node in a specific layer
            template<typename Layer_type>
            void setActiveCamera(RandysEngine::Layer_Node& node){
                
                if(node.isValid){

                    auto itBegin = layers.begin();
                    auto itEnd = layers.end();
                    while(itBegin != itEnd){
                        if(std::holds_alternative<Layer_type>(*itBegin)){
                            Layer_type* layer = &std::get<Layer_type>(*itBegin);
                            if(node.layerId == layer->getInstance()){

                                layer->setActiveCamera(node);
                                break;
                            }
                        }
                        itBegin++;
                    }
                }
            }

            //Create a node in a specific layer
            template<typename Layer_type>
            unsigned int addMesh(RandysEngine::Layer_Node& node, std::string file){
                
                unsigned int devolver = RandysEngine::Model_Entity::MAXMESHES;

                if(node.isValid){
                    Layer_type* layer;

                    

                    auto itBegin = layers.begin();
                    auto itEnd = layers.end();
                    while(itBegin != itEnd){
                        if(std::holds_alternative<Layer_type>(*itBegin)){
                            layer = &std::get<Layer_type>(*itBegin);
                            if(layer->getInstance() == node.layerId){
                                RandysEngine::Model_Entity* model = layer->getModel(node);

                                if(model){
                                    for(unsigned int i = 0; i < RandysEngine::Model_Entity::MAXMESHES;i++){
                                        if(!model->hasMesh[i]){
                                            model->hasMesh[i] = true;
                                            if(!meshExists(file)){
#ifndef __3DS__
                                                auto key = ResourceManager.createResource<gl_mesh_resource>(file);
#else
                                                auto key = ResourceManager.createResource<citro_mesh_resource>(file);
#endif
                                                resources.emplace(std::make_pair(file, key));
                                                model->meshes[i] = key;
                                            }
                                            else{
                                                auto key = resources.find(file)->second;
                                                model->meshes[i] = key;
                                            }
                                            devolver = i;
                                            break;
                                        }
                                    }
                                }

                                break;
                            }
                        }
                        itBegin++;
                    }
                }
                return devolver;
            }

            //
            void setTranslateNode(RandysEngine::Layer_Node& node, float x, float y, float z) noexcept;
            void setScaleNode(RandysEngine::Layer_Node& node, float x, float y, float z) noexcept;
            void setRotateNode(RandysEngine::Layer_Node& node, float x, float y, float z) noexcept;

            void TranslateNode(RandysEngine::Layer_Node& node, float x, float y, float z) noexcept;
            void ScaleNode(RandysEngine::Layer_Node& node, float x, float y, float z) noexcept;
            void RotateNode(RandysEngine::Layer_Node& node, float x, float y, float z) noexcept;

            std::array<float,3> getTranslationNode(RandysEngine::Layer_Node& node) noexcept;
            std::array<float,3> getScalationNode(RandysEngine::Layer_Node& node) noexcept;
            std::array<float,3> getRotationNode(RandysEngine::Layer_Node& node) noexcept;


            //Create a node in a specific layer
            template<typename Layer_type>
            void addTexture(RandysEngine::Layer_Node& node, unsigned int meshNumber, std::string file){
                
                if(node.isValid && meshNumber != RandysEngine::Model_Entity::MAXMESHES){
                    Layer_type* layer;

                    auto itBegin = layers.begin();
                    auto itEnd = layers.end();
                    while(itBegin != itEnd){
                        if(std::holds_alternative<Layer_type>(*itBegin)){
                            layer = &std::get<Layer_type>(*itBegin);
                            if(layer->getInstance() == node.layerId){
                                RandysEngine::Model_Entity* model = layer->getModel(node);
                                if(model){
                                    model->hasTexture[meshNumber] = true;
                                    if(!textureExists(file)){
#ifndef __3DS__
                                        auto key = ResourceManager.createResource<gl_texture_resource>(file);
#else
                                        auto key = ResourceManager.createResource<citro_texture_resource>(file);
#endif
                                        resources.emplace(std::make_pair(file, key));
                                        model->textures[meshNumber] = key;
                                    }
                                    else{
                                        auto key = resources.find(file)->second;
                                        model->textures[meshNumber] = key;
                                    }
                                }

                                break;
                            }
                        }
                        itBegin++;
                    }
                }
            }

            using indexVector = std::vector<
                unsigned int, 
                RandysEngine::Pool::Static_pool_allocator<unsigned int,20>
                >;

            //Get a list of the specific type of layer and their indexes
            template<typename Layer_type> 
            indexVector getLayerIndexes(){
                indexVector devolver;

                devolver.reserve(20);

                auto itBegin = layers.begin();
                auto itEnd = layers.end();
                while(itBegin != itEnd){
                    if(std::holds_alternative<Layer_type>(*itBegin)){
                        Layer_type* layer;
                        layer = &std::get<Layer_type>(*itBegin);
                        devolver.push_back(layer->getInstance());
                    }
                    itBegin++;
                }
                
                return devolver;
            }
            
            //Run the frame, not only drawing it but interacting with it
            void runFrame();

            bool meshExists(std::string file);

            bool reserveMeshResource(std::string file);
            bool freeMeshResource(std::string file);

            bool textureExists(std::string file);

            bool reserveTextureResource(std::string file);
            bool freeTextureResource(std::string file);

            bool addTextureSkybox(std::size_t layerId, std::string file);

            bool addSprite(RandysEngine::Layer_Node& node, float x, float y, float width, float height);

            bool setRotationCameraSKybox(std::size_t LayerId, float rotX, float rotY, float rotZ);

            bool rotateCameraSKybox(std::size_t LayerId, float rotX, float rotY, float rotZ);

            //Read the input from the key
            bool readKeyPressed(KeyInput input) const{
                return screen.getInputPressed(input);
            }

            bool isAppRunning() const{
                return screen.isAppRunning();
            }

            void closeApp(){
                screen.closeApp();
            }
    };

};