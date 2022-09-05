#include "minitree.hpp"


namespace RandysEngine{

    bool layer_minitree::draw() const{

        bool devolver = true;
        if(!activated){
            std::cout << "Cannot draw deactivated layer\n";
                devolver = false;
            }
        else{
            for(SlotMap::SlotMap_Index_Type i = 0;i < models.current_size();i++){
                auto& model = *models.atPosition(i);
                #ifndef __3DS__
                    auto& meshResource = *resource_manager.getResource<gl_mesh_resource>(model.mesh_resource);
                #else
                    auto& meshResource = *resource_manager.getResource<citro_mesh_resource>(model.mesh_resource);
                #endif
                
                meshResource.draw();
            }
        }
        return devolver;

    }

    RandysEngine::MinitreeNode* layer_minitree::getNode(RandysEngine::Layer_Element input) const noexcept{
        RandysEngine::MinitreeNode* devolver = nullptr;

        if(input.layerId == this->instance){
            devolver = nodes.atPosition(input.element);
        }

        return devolver;
    }

    RandysEngine::Model_Entity* layer_minitree::getModel(RandysEngine::Layer_Element input) const noexcept{

        RandysEngine::Model_Entity* devolver = nullptr;

        if(input.layerId == this->instance){
            RandysEngine::MinitreeNode* node;
            node = nodes.atPosition(input.element);
            if(node->type_entity == RandysEngine::entityType_enum::model){
                devolver = models.atPosition(node->entity);
            }
        }

        return devolver;
    }

    RandysEngine::Layer_Element layer_minitree::addModel() noexcept{
        RandysEngine::Layer_Element devolver;

        if(models.current_size() != models.max_capacity() && nodes.current_size()!= nodes.max_capacity()){

            auto& rootNodeItem = *nodes.atPosition(rootNode); 

            for(unsigned int i = 0; i < RandysEngine::MinitreeNode::maxChildren;i++){
                if(!rootNodeItem.hasChildren[i]){

                    RandysEngine::MinitreeNode node;
                    RandysEngine::Model_Entity model;
                    model.mesh_resource = triangle_Mesh;


                    node.hasParent = true;
                    node.parentNode = rootNode;

                    node.type_entity = RandysEngine::entityType_enum::model;
                    auto node_key = node.entity = models.push_back(model);

                    rootNodeItem.childrenNodes[i] = nodes.push_back(node);
                    rootNodeItem.hasChildren[i] = true;

                    devolver.element = node_key;
                    devolver.layerId = this->instance;

                    break;
                }
            }


        }

        return devolver;
    }

}