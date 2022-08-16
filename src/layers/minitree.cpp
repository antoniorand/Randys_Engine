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
                //auto& meshResource = *resource_manager.getResource<gl_mesh_resource>(model.mesh_resource);
                //meshResource.draw();
            }
        }
        return devolver;

    }

    bool layer_minitree::addModel(){
        bool devolver = false;

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
                    node.entity = models.push_back(model);

                    rootNodeItem.childrenNodes[i] = nodes.push_back(node);
                    rootNodeItem.hasChildren[i] = true;

                    devolver = true;

                    break;
                }
            }


        }

        return devolver;
    }

}