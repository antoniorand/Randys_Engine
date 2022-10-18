#include "GUI.hpp"

namespace RandysEngine{

    layer_GUI::layer_GUI(ResourceManager& resource_Manager) 
        : layer_interface<layer_GUI>(resource_Manager), nodes{maxNodes},
        sprites{maxNodes},matrixes{maxNodes}{

        GUINode e_rootNode;

#ifndef __3DS__
        e_rootNode.matrixKey = matrixes.push_back(gl_matrix{});
#else
        e_rootNode.matrixKey = matrixes.push_back(citro_matrix{});
#endif
        rootNode = nodes.push_back(e_rootNode);
        

    };

    [[nodiscard]] const RandysEngine::Layer_Node layer_GUI::createNode() noexcept{
        RandysEngine::Layer_Node devolver;
        
        auto& rootNodeItem = *nodes.atPosition(rootNode); 
        
        for(unsigned int i = 0; i < RandysEngine::GUINode::maxChildren;i++){
                if(!rootNodeItem.hasChildren[i]){

                    RandysEngine::GUINode node;
#ifndef __3DS__
                    node.matrixKey = matrixes.push_back(gl_matrix{});
#else
                    node.matrixKey = matrixes.push_back(citro_matrix{});
#endif
                    node.hasParent = true;
                    node.parentNode = rootNode;

                    rootNodeItem.childrenNodes[i] = nodes.push_back(node);
                    rootNodeItem.hasChildren[i] = true;

                    devolver.layerId = this->instance;
                    devolver.isValid = true;
                    devolver.reference = rootNodeItem.childrenNodes[i];
                    break;
                }
            }
        return devolver;
    }

    [[nodiscard]] const RandysEngine::Layer_Node layer_GUI::createNode(RandysEngine::Layer_Node& input){
        RandysEngine::Layer_Node devolver;
        
        if(input.layerId == this->instance && input.isValid){

            auto oldNode = nodes.atPosition(input.reference); 
            
            if(!oldNode){
                input.isValid = false;
            }
            else{
                for(unsigned int i = 0; i < RandysEngine::GUINode::maxChildren;i++){
                    if(!oldNode->hasChildren[i]){

                        RandysEngine::GUINode node;
#ifndef __3DS__
                        node.matrixKey = matrixes.push_back(gl_matrix{});
#else
                        node.matrixKey = matrixes.push_back(citro_matrix{});
#endif
                        node.hasParent = true;
                        node.parentNode = rootNode;

                        oldNode->childrenNodes[i] = nodes.push_back(node);
                        oldNode->hasChildren[i] = true;

                        devolver.layerId = this->instance;
                        devolver.isValid = true;
                        devolver.reference = oldNode->childrenNodes[i];
                        break;
                    }
                }
            }


        } 
        return devolver;
    }

    RandysEngine::Sprite_Entity* layer_GUI::getSprite(RandysEngine::Layer_Node& input) const noexcept{

        RandysEngine::Sprite_Entity* devolver = nullptr;

        if(input.layerId == this->instance && input.isValid){

            auto oldNode = nodes.atPosition(input.reference);

            if(!oldNode){
                input.isValid = false;
            }
            else{
                devolver = sprites.atPosition(oldNode->sprite);
            }

        }

        return devolver;

    }

    void layer_GUI::addSprite(RandysEngine::Layer_Node& input, float x, float y, float width, float height) noexcept{

        if(input.layerId == this->instance && input.isValid && sprites.current_size() != sprites.max_capacity()){

            auto oldNode = nodes.atPosition(input.reference);

            if(!oldNode){
                input.isValid = false;
            }
            else{

                RandysEngine::Sprite_Entity newSprite = Sprite_Entity(x,y,width,height);


                oldNode->sprite = sprites.push_back(newSprite);
                oldNode->hasSprite = true;

            }
        }

    }

}