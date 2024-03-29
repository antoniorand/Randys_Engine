#include <layers/skybox.hpp>

namespace RandysEngine{

    bool layer_skybox::draw(
#ifndef __3DS__
        RandysEngine::gl_shader* shader
#else
        RandysEngine::citro_shader* shader
#endif
    ){

        bool devolver = true;
        if(!activated){
            //std::cout << "Cannot draw deactivated layer\n";
            devolver = false;
        }
        else{

#ifndef __3DS__
            RandysEngine::gl_skybox_helper helper;
            auto& meshDraw = *resource_manager.getResource<gl_mesh_resource>(skybox.skyboxCube);
            gl_matrix modelMatrix{};
            auto textureDraw = resource_manager.getResource<gl_texture_resource>(skybox.texture);
#else
            RandysEngine::citro_skybox_helper helper;
            auto& meshDraw = *resource_manager.getResource<citro_mesh_resource>(skybox.skyboxCube);
            citro_matrix modelMatrix{};
            auto textureDraw = resource_manager.getResource<citro_texture_resource>(skybox.texture);
#endif
            modelMatrix.scalation[0] = 100.0;
            modelMatrix.scalation[1] = 100.0;
            modelMatrix.scalation[2] = 100.0;
            helper.reverseCull();
            shader->setMat4("model",modelMatrix);
            shader->setMat4("projection", skybox.camera.projectionMatrix);
            shader->setMat4("view", skybox.camera.viewMatrix);

            if(!textureDraw){
                skybox.hasTexture = false;
            }

            if(skybox.hasTexture){
                textureDraw->use();
                meshDraw.draw();
                textureDraw->unlink();
            }
            else{
                meshDraw.draw();
            }
            helper.restoreCull();

        }
        return devolver;

    }

    void layer_skybox::setRotationCameraSkyBox(float x, float y, float z) noexcept{
        skybox.camera.viewMatrix.rotation[0] = x;
        skybox.camera.viewMatrix.rotation[1] = y;
        skybox.camera.viewMatrix.rotation[2] = z;
        skybox.camera.viewMatrix.changed = true;
    }

    void layer_skybox::rotateCameraSkyBox(float x, float y, float z) noexcept{
        skybox.camera.viewMatrix.rotation[0] += x;
        skybox.camera.viewMatrix.rotation[1] += y;
        skybox.camera.viewMatrix.rotation[2] += z;
        skybox.camera.viewMatrix.changed = true;
    }

    void layer_skybox::changeTexture(RandysEngine::ResourceManager::KeyId input){

#ifndef __3DS__
        auto& texture = *resource_manager.getResource<gl_texture_resource>(input);
#else
        auto& texture = *resource_manager.getResource<citro_texture_resource>(input);
#endif
        texture.setToSkybox();
        skybox.texture = input;
        skybox.hasTexture = true;
    }

}

