#include <layers/minitree.hpp>


namespace RandysEngine{

    layer_minitree::layer_minitree(ResourceManager& man) : layer_interface<layer_minitree>(man), nodes{maxNodesMinitree},
        models{maxModelsMinitree}, lights{maxLightsMinitree}, 
        cameras{maxCamerasMinitree}, matrixes{maxNodesMinitree + 10}{
        
            MinitreeNode e_rootNode;
#ifndef __3DS__
            e_rootNode.matrixKey = matrixes.push_back(gl_matrix{});
#else
            e_rootNode.matrixKey = matrixes.push_back(citro_matrix{});
#endif
            rootNode = nodes.push_back(e_rootNode);

    }

    bool layer_minitree::draw(
#ifndef __3DS__
                RandysEngine::gl_shader* shader
#else
                RandysEngine::citro_shader* shader
#endif
    ){

        bool devolver = true;
        if(!activated){
            std::cout << "Cannot draw deactivated layer\n";
                devolver = false;
        }
        else{

            runLinkedMovement();

            if(isCameraActive){
                auto& activeCamera = *cameras.atPosition(currentActiveCamera);
                
                auto& projectionM = *matrixes.atPosition(activeCamera.projectionMatrix);
                auto viewM = matrixes.atPosition(activeCamera.matrixKey)->reverse();

                shader->setMat4("projection",projectionM);
                shader->setMat4("view", viewM);

            }

            for(SlotMap::SlotMap_Index_Type i = 0;i < models.current_size();i++){
                auto& model = *models.atPosition(i);
                auto& matrix = *matrixes.atPosition(model.matrixKey);

                for(unsigned int i = 0; i < Model_Entity::MAXMESHES; i++){
                    if(model.hasMesh[i]){
#ifndef __3DS__                                                
                        auto meshResource = resource_manager.getResource<gl_mesh_resource>(model.meshes[i]);
#else
                        auto meshResource = resource_manager.getResource<citro_mesh_resource>(model.meshes[i]);
#endif
                        
                        shader->setMat4("model",matrix);

                        if(!meshResource){
                            model.hasMesh[i] = false;
                        }
                        else if(model.hasTexture[i]){
#ifndef __3DS__
                            auto textureResource = resource_manager.getResource<gl_texture_resource>(model.textures[i]); 
#else 
                            auto textureResource = resource_manager.getResource<citro_texture_resource>(model.textures[i]); 
#endif
                                
                            if(!textureResource)
                                model.hasTexture[i] = false;
                            else{
                                textureResource->use();
                                meshResource->draw();
                                textureResource->unlink();
                            }

                        }
                        else{
                            meshResource->draw();
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
        layer_minitree::createNode(RandysEngine::Layer_Node& input) noexcept{

        RandysEngine::Layer_Node devolver;
        
        if(input.layerId == this->instance && input.isValid){

            auto oldNode = nodes.atPosition(input.reference); 
            
            if(!oldNode){
                input.isValid = false;
            }
            else{
                for(unsigned int i = 0; i < RandysEngine::MinitreeNode::maxChildren;i++){
                    if(!oldNode->hasChildren[i]){

                        RandysEngine::MinitreeNode node;

#ifndef __3DS__
                        node.matrixKey = matrixes.push_back(gl_matrix{});
#else
                        node.matrixKey = matrixes.push_back(citro_matrix{});
#endif

                        node.hasParent = true;
                        node.parentNode = input.reference;

                        node.type_entity = RandysEngine::entityType_enum::none;

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

    void layer_minitree::deleteLinkedNodes(const SlotMap::SlotMap_Key node){
        //copy the node
        MinitreeNode nodeToDelete = *nodes.atPosition(node);

        for(unsigned int i = 0; i < MinitreeNode::maxChildren;i++){
            
            if(nodeToDelete.hasChildren[i]){
                deleteLinkedNodes(nodeToDelete.childrenNodes[i]);
            }

        }
        nodes.erase(node);
        matrixes.erase(nodeToDelete.matrixKey);

        switch(nodeToDelete.type_entity){
            case RandysEngine::entityType_enum::model:
                models.erase(nodeToDelete.entity);
            break;
            case RandysEngine::entityType_enum::light:
                lights.erase(nodeToDelete.entity);
            break;
            case RandysEngine::entityType_enum::camera:
                cameras.erase(nodeToDelete.entity);
                if(nodeToDelete.entity == this->currentActiveCamera){
                    this->isCameraActive = false;
                }
            break;
            default: 
            break;
        }

    }

    [[nodiscard]] bool layer_minitree::deleteNode(RandysEngine::Layer_Node& input) noexcept{

        bool devolver =  false;

        if(input.layerId == this->instance && input.isValid){
            
            //copy the node
            MinitreeNode node = *nodes.atPosition(input.reference);

            if(node.hasParent){

                auto& parentNode = *nodes.atPosition(node.parentNode);
                
                for(unsigned int i = 0; i < MinitreeNode::maxChildren;i++){
                    if(input.reference == parentNode.childrenNodes[i] && parentNode.hasChildren[i]){
                        parentNode.hasChildren[i] = false;
                        break;
                    }
                }

            }

            deleteLinkedNodes(input.reference);
            input.isValid = false;

        }

        return devolver;

    }

    void layer_minitree::deleteEntity(RandysEngine::MinitreeNode& node){

        switch(node.type_entity){
            case RandysEngine::entityType_enum::camera:
                cameras.erase(node.entity);
            break;
            case RandysEngine::entityType_enum::model:
                models.erase(node.entity);
            break;
            default:
            break;
        }

        node.type_entity = RandysEngine::entityType_enum::none;

    }

    void layer_minitree::addModel(RandysEngine::Layer_Node& input) noexcept{
        
        if(input.layerId == this->instance && input.isValid && models.current_size() != models.max_capacity()){

            auto oldNode = nodes.atPosition(input.reference);

            if(!oldNode){
                input.isValid = false;
            }
            else{

                RandysEngine::Model_Entity newModel;

                newModel.matrixKey = oldNode->matrixKey;

                deleteEntity(*oldNode);

                oldNode->type_entity = RandysEngine::entityType_enum::model;

                oldNode->entity = models.push_back(newModel);

            }
        }

    }

    void layer_minitree::addCamera(RandysEngine::Layer_Node& input, 
        float fov, float aspect, float near, float far) noexcept{
        
        if(input.layerId == this->instance && input.isValid && cameras.current_size() != cameras.max_capacity()){

            auto oldNode = nodes.atPosition(input.reference);

            if(!oldNode){
                input.isValid = false;
            }
            else{

                RandysEngine::Camera_Entity newCamera;

#ifndef __3DS__
                gl_matrix newProjection{};
#else
                citro_matrix newProjection{};
                newProjection.rotation[1] = 3.14;
                newProjection.scalation[0] = -1.0; 
#endif

                newProjection.perspective = true;
                newProjection.fov = fov;
                newProjection.aspect = aspect;
                newProjection.near = near;
                newProjection.far = far;

                newCamera.projectionMatrix = matrixes.push_back(newProjection);

                newCamera.matrixKey = oldNode->matrixKey;

                deleteEntity(*oldNode);

                oldNode->type_entity = RandysEngine::entityType_enum::camera;

                oldNode->entity = cameras.push_back(newCamera);
            }
        }

    }

    bool layer_minitree::setActiveCamera(RandysEngine::Layer_Node& node) noexcept{
        
        bool devolver = false;
        
        if(node.isValid && node.layerId == this->instance){

            auto oldNode = nodes.atPosition(node.reference);

            if(!oldNode){
                node.isValid = false;
            }
            else{
                if(oldNode->type_entity == RandysEngine::entityType_enum::camera){
                    devolver = true;
                    this->isCameraActive = true;
                    this->currentActiveCamera = oldNode->entity;
                }
            }

        }

        return devolver;
    }

    RandysEngine::Model_Entity* 
        layer_minitree::getModel(RandysEngine::Layer_Node& input) const noexcept{

        RandysEngine::Model_Entity* devolver = nullptr;

        if(input.layerId == this->instance && input.isValid){

            auto oldNode = nodes.atPosition(input.reference);

            if(!oldNode){
                input.isValid = false;
            }
            else{
                if(oldNode->type_entity == RandysEngine::entityType_enum::model){
                    devolver = models.atPosition(oldNode->entity);
                }
            }

        }

        return devolver;

    }

    bool layer_minitree::setTranslationMatrix(RandysEngine::Layer_Node& input,float x, float y, float z) const noexcept{
        bool devolver = false;
        if(input.layerId == this->instance && input.isValid){
            
            auto oldNode = nodes.atPosition(input.reference);
            if(!oldNode){
                input.isValid = false;
            }
            else{
                auto& matrix_to_translate = *matrixes.atPosition(oldNode->matrixKey);

                matrix_to_translate.translation[0] = x;
                matrix_to_translate.translation[1] = y;
                matrix_to_translate.translation[2] = z;
                matrix_to_translate.changed = true;

                devolver = true;
            }

        }
        return devolver;
    }

    bool layer_minitree::setScalationMatrix(RandysEngine::Layer_Node& input,float x, float y, float z) const noexcept{
        bool devolver = false;
        if(input.layerId == this->instance && input.isValid){

            auto oldNode = nodes.atPosition(input.reference);
            if(!oldNode){
                input.isValid = false;
            }
            else{
                auto& matrix_to_translate = *matrixes.atPosition(oldNode->matrixKey);

                matrix_to_translate.scalation[0] = x;
                matrix_to_translate.scalation[1] = y;
                matrix_to_translate.scalation[2] = z;
                matrix_to_translate.changed = true;

                devolver = true;
            }

        }
        return devolver;
    }

    bool layer_minitree::setRotationMatrix(RandysEngine::Layer_Node& input,float x, float y, float z) const noexcept{
        bool devolver = false;
        if(input.layerId == this->instance && input.isValid){

            auto oldNode = nodes.atPosition(input.reference);
            if(!oldNode){
                input.isValid = false;
            }
            else{
                auto& matrix_to_translate = *matrixes.atPosition(oldNode->matrixKey);

                matrix_to_translate.rotation[0] = x;
                matrix_to_translate.rotation[1] = y;
                matrix_to_translate.rotation[2] = z;
                matrix_to_translate.changed = true;

                devolver = true;
            }

        }
        return devolver;
    }

    bool layer_minitree::TranslateMatrix(RandysEngine::Layer_Node& input,float x, float y, float z) const noexcept{
        bool devolver = false;
        if(input.layerId == this->instance && input.isValid){

            auto oldNode = nodes.atPosition(input.reference);

            if(!oldNode){
                input.isValid = false;
            }
            else{
                auto& matrix_to_translate = *matrixes.atPosition(oldNode->matrixKey);

                matrix_to_translate.translation[0] += x;
                matrix_to_translate.translation[1] += y;
                matrix_to_translate.translation[2] += z;
                matrix_to_translate.changed = true;

                devolver = true;
            }


        }
        return devolver;
    }

    bool layer_minitree::ScaleMatrix(RandysEngine::Layer_Node& input,float x, float y, float z) const noexcept{
        bool devolver = false;
        if(input.layerId == this->instance && input.isValid){

            auto oldNode = nodes.atPosition(input.reference);

            if(!oldNode){
                input.isValid = false;
            }
            else{
                auto& matrix_to_translate = *matrixes.atPosition(oldNode->matrixKey);

                matrix_to_translate.scalation[0] += x;
                matrix_to_translate.scalation[1] += y;
                matrix_to_translate.scalation[2] += z;
                matrix_to_translate.changed = true;

                devolver = true;
            }


        }
        return devolver;
    }

    bool layer_minitree::RotateMatrix(RandysEngine::Layer_Node& input,float x, float y, float z) const noexcept{
        bool devolver = false;
        if(input.layerId == this->instance && input.isValid){

            auto oldNode = nodes.atPosition(input.reference);

            if(!oldNode){
                input.isValid = false;
            }
            else{
                auto& matrix_to_translate = *matrixes.atPosition(oldNode->matrixKey);

                matrix_to_translate.rotation[0] += x;
                matrix_to_translate.rotation[1] += y;
                matrix_to_translate.rotation[2] += z;
                matrix_to_translate.changed = true;

                devolver = true;
            }


        }
        return devolver;
    }

    std::array<float,3> layer_minitree::getTranslationMatrix(RandysEngine::Layer_Node& input) const noexcept{
        std::array<float,3> devolver = {0.0f,0.0f,0.0f};
        if(input.layerId == this->instance && input.isValid){

            auto oldNode = nodes.atPosition(input.reference);
            if(!oldNode){
                input.isValid = false;
            }
            else{
                const auto& matrix_to_translate = *matrixes.atPosition(oldNode->matrixKey);

                devolver[0] = matrix_to_translate.translation[0];
                devolver[1] = matrix_to_translate.translation[1];
                devolver[2] = matrix_to_translate.translation[2];
            }

        }
        return devolver;
    }

    std::array<float,3> layer_minitree::getScalationMatrix(RandysEngine::Layer_Node& input) const noexcept{
        std::array<float,3> devolver = {0.0f,0.0f,0.0f};
        if(input.layerId == this->instance && input.isValid){

            auto oldNode = nodes.atPosition(input.reference);
            if(!oldNode){
                input.isValid = false;
            }
            else{
                const auto& matrix_to_translate = *matrixes.atPosition(oldNode->matrixKey);

                devolver[0] = matrix_to_translate.scalation[0];
                devolver[1] = matrix_to_translate.scalation[1];
                devolver[2] = matrix_to_translate.scalation[2];
            }

        }
        return devolver;
    }

    std::array<float,3> layer_minitree::getRotationMatrix(RandysEngine::Layer_Node& input) const noexcept{
        std::array<float,3> devolver = {0.0f,0.0f,0.0f};
        if(input.layerId == this->instance && input.isValid){

            auto oldNode = nodes.atPosition(input.reference);
            if(!oldNode){
                input.isValid = false;
            }
            else{
                const auto& matrix_to_translate = *matrixes.atPosition(oldNode->matrixKey);

                devolver[0] = matrix_to_translate.rotation[0];
                devolver[1] = matrix_to_translate.rotation[1];
                devolver[2] = matrix_to_translate.rotation[2];
            }

        }
        return devolver;
    }

    void layer_minitree::runLinks(RandysEngine::MinitreeNode& node){

        auto& matrix = *matrixes.atPosition(node.matrixKey);

        for(unsigned int i = 0; i < RandysEngine::MinitreeNode::maxChildren;i++){
            if(node.hasChildren[i]){
                auto& childNode = *nodes.atPosition(node.childrenNodes[i]);
                auto& childMatrix = *matrixes.atPosition(childNode.matrixKey);

                childMatrix.multiply(matrix);
                
                runLinks(childNode);
            }
        }
    }

    void layer_minitree::runLinkedMovement(){
        for(unsigned int i = 0; i < nodes.current_size();i++){
            auto& node = *nodes.atPosition(i);
            auto& matrix = *matrixes.atPosition(node.matrixKey);

            if(matrix.changed){
                runLinks(node);
            }
        }
    }

}