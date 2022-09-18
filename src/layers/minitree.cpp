#include "minitree.hpp"


namespace RandysEngine{

    bool layer_minitree::draw(
#ifndef __3DS__
                RandysEngine::gl_shader* shader
#else
                RandysEngine::citro_shader* shader
#endif
    ) const{

        bool devolver = true;
        if(!activated){
            std::cout << "Cannot draw deactivated layer\n";
                devolver = false;
            }
        else{
            for(SlotMap::SlotMap_Index_Type i = 0;i < models.current_size();i++){
                auto& model = *models.atPosition(i);
                auto& matrix = *matrixes.atPosition(model.matrixKey);
                shader->setMat4("transform",matrix);

                for(unsigned int i = 0; i < Model_Entity::MAXMESHES; i++){
                    if(model.hasMesh[i]){
#ifndef __3DS__                                                
                        auto& meshResource = *resource_manager.getResource<gl_mesh_resource>(model.meshes[i]);
#else
                        auto& meshResource = *resource_manager.getResource<citro_mesh_resource>(model.meshes[i]);
#endif
                        
                    
                        if(model.hasTexture[i]){
#ifndef __3DS__
                            auto& textureResource = *resource_manager.getResource<gl_texture_resource>(model.textures[i]); 
#else 
                            auto& textureResource = *resource_manager.getResource<citro_texture_resource>(model.textures[i]); 
#endif
                                
                                textureResource.use();
                                meshResource.draw();
                                textureResource.unlink();
                            }
                            else{
                                meshResource.draw();
                            }

                        }
                                
                    }
            }
        }
        return devolver;
    }

    

    [[nodiscard]] const RandysEngine::Layer_Node layer_minitree::createNode() noexcept{

        RandysEngine::Layer_Node devolver;
        
        auto& rootNodeItem = *nodes.atPosition(rootNode); 
        
        for(unsigned int i = 0; i < RandysEngine::MinitreeNode::maxChildren;i++){
                if(!rootNodeItem.hasChildren[i]){

                    RandysEngine::MinitreeNode node;

#ifndef __3DS__
                    node.matrixKey = matrixes.push_back(gl_matrix{});
#else
                    node.matrixKey = matrixes.push_back(citro_matrix{});
#endif

                    node.hasParent = true;
                    node.parentNode = rootNode;

                    node.type_entity = RandysEngine::entityType_enum::none;

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

    [[nodiscard]] const RandysEngine::Layer_Node 
        layer_minitree::createNode(RandysEngine::Layer_Node input) noexcept{

        RandysEngine::Layer_Node devolver;
        
        if(input.layerId == this->instance && input.isValid){

            auto& oldNode = *nodes.atPosition(input.reference); 
            
            for(unsigned int i = 0; i < RandysEngine::MinitreeNode::maxChildren;i++){
                if(!oldNode.hasChildren[i]){

                    RandysEngine::MinitreeNode node;

#ifndef __3DS__
                    node.matrixKey = matrixes.push_back(gl_matrix{});
#else
                    node.matrixKey = matrixes.push_back(citro_matrix{});
#endif

                    node.hasParent = true;
                    node.parentNode = input.reference;

                    node.type_entity = RandysEngine::entityType_enum::none;

                    oldNode.childrenNodes[i] = nodes.push_back(node);
                    oldNode.hasChildren[i] = true;

                    devolver.layerId = this->instance;
                    devolver.isValid = true;
                    devolver.reference = oldNode.childrenNodes[i];
                    break;
                }
            }
        } 
        return devolver;
    }

    void layer_minitree::addModel(const RandysEngine::Layer_Node input) noexcept{
        
        if(input.layerId == this->instance && input.isValid){

            auto& oldNode = *nodes.atPosition(input.reference);

            RandysEngine::Model_Entity newModel;

            newModel.matrixKey = oldNode.matrixKey;

            oldNode.type_entity = RandysEngine::entityType_enum::model;

            oldNode.entity = models.push_back(newModel);
        }

    }

    RandysEngine::Model_Entity* 
        layer_minitree::getModel(const RandysEngine::Layer_Node input) const noexcept{

        RandysEngine::Model_Entity* devolver = nullptr;

        if(input.layerId == this->instance && input.isValid){

            auto& oldNode = *nodes.atPosition(input.reference);

            if(oldNode.type_entity == RandysEngine::entityType_enum::model){

                devolver = models.atPosition(oldNode.entity);

            }
        }

        return devolver;

    }

    bool layer_minitree::setTranslationMatrix(const RandysEngine::Layer_Node input,float x, float y, float z) const noexcept{
        bool devolver = false;
        if(input.layerId == this->instance && input.isValid){

            auto& oldNode = *nodes.atPosition(input.reference);
            auto& matrix_to_translate = *matrixes.atPosition(oldNode.matrixKey);

            matrix_to_translate.translation[0] = x;
            matrix_to_translate.translation[1] = y;
            matrix_to_translate.translation[2] = z;
            matrix_to_translate.changed = true;

            devolver = true;
        }
        return devolver;
    }



}