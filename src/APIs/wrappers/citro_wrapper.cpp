
//#ifdef __3DS__
#include "citro_wrapper.hpp"
namespace RandysEngine{

    #define DISPLAY_TRANSFER_FLAGS \
	(GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(0) | GX_TRANSFER_RAW_COPY(0) | \
	GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8) | \
	GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO))


    // Helper function for loading a texture from memory
    bool loadTextureFromFile(C3D_Tex* tex, C3D_TexCube* cube,
        const char* name){
        bool devolver = false;
        FILE* file = fopen(name,"rb");
        if(file!=NULL){
            Tex3DS_Texture t3x = 
            Tex3DS_TextureImportStdio(file, tex,NULL,false);
            if (!t3x){
                
                fclose(file);
            }
            else{
                // Delete the t3x object since we don't need it
                Tex3DS_TextureFree(t3x);
                devolver = true;
            }
        }
        if(devolver)
            printf("successful!\n");
        return devolver;
    }

    citro_texture_resource::citro_texture_resource(std::string file) noexcept{
        // Load the texture and bind it to the first texture unit
	    if (!loadTextureFromFile(&texture, NULL, "romfs:/gfx/face.t3x"))
	    	svcBreak(USERBREAK_PANIC);
	    C3D_TexSetFilter(&texture, GPU_LINEAR, GPU_NEAREST);
    }

    citro_texture_resource::~citro_texture_resource() noexcept{
        // Free the texture
	    C3D_TexDelete(&texture);
    }

    void citro_texture_resource::use() noexcept{
        C3D_TexBind(0, &texture);
    }

    void citro_texture_resource::unlink() noexcept{
        C3D_TexBind(0, &default_texture);
    }

    Vertex verticesConverter(Vertex vertex){   
        vertex.x = 200*vertex.x + 200;    
        vertex.y = 120*vertex.y + 120;    
        return vertex;
    }

    citro_mesh_resource::citro_mesh_resource(std::string input) noexcept{
        Vertex convertedVertices[countVertices];

        for(unsigned int i = 0; i < countVertices; i++){
            convertedVertices[i] = verticesConverter(vertices[i]);
        }

        vbo_data = linearAlloc(sizeVertices);
        memcpy(vbo_data,convertedVertices, sizeVertices);

        ibo_data = linearAlloc(sizeIndices);
        memcpy(ibo_data, indices_list, sizeIndices);

    }

    citro_mesh_resource::~citro_mesh_resource() noexcept{
        linearFree(vbo_data);
        linearFree(ibo_data);
    }

    void citro_mesh_resource::draw() const noexcept{

        // Configure buffers
        C3D_BufInfo* bufInfo = C3D_GetBufInfo();
        BufInfo_Init(bufInfo);
        BufInfo_Add(bufInfo, vbo_data, sizeof(Vertex), 2, 0x10);

        // Draw the VBO
        //C3D_DrawArrays(GPU_TRIANGLES, 0, numberVertices);
        C3D_DrawElements(GPU_TRIANGLES,countIndices,C3D_UNSIGNED_SHORT,ibo_data);

    }

    citro_shader::citro_shader() noexcept{
        // Load the vertex shader, create a shader program and bind it
        vshader_dvlb = DVLB_ParseFile((u32*)vshader_shbin,
        vshader_shbin_size);
        shaderProgramInit(&shaderProgram);
        shaderProgramSetVsh(&shaderProgram, &vshader_dvlb->DVLE[0]);
        C3D_BindProgram(&shaderProgram);

        // Get the location of the uniforms
	    uLoc_projection = shaderInstanceGetUniformLocation(shaderProgram.vertexShader, "projection");
        
        // Configure attributes for use with the vertex shader
        C3D_AttrInfo* attrInfo = C3D_GetAttrInfo();
        AttrInfo_Init(attrInfo);
        AttrInfo_AddLoader(attrInfo, 0, GPU_FLOAT, 3); // v0=position
        AttrInfo_AddLoader(attrInfo, 1, GPU_FLOAT, 2); // v1=position

        // Compute the projection matrix
        Mtx_OrthoTilt(&projection, 0.0, 400.0, 0.0, 240.0, 0.0, 1.0, true);
        
        // Configure the first fragment shading substage to just pass through the vertex color
        // See https://www.opengl.org/sdk/docs/man2/xhtml/glTexEnv.xml for more insight
        C3D_TexEnv* env = C3D_GetTexEnv(0);
        C3D_TexEnvInit(env);

        C3D_TexEnvSrc(env, C3D_Both, GPU_TEXTURE0, GPU_PRIMARY_COLOR, (GPU_TEVSRC)0);
	    C3D_TexEnvFunc(env, C3D_Both, GPU_MODULATE);
    }

    citro_shader::~citro_shader() noexcept{
        shaderProgramFree(&shaderProgram);
        DVLB_Free(vshader_dvlb);
    }

    void citro_shader::useShader() const noexcept{
        // Update the uniforms
    	C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uLoc_projection, &projection);
    }
    void citro_shader::setBool(const std::string &name, bool value) const{
        //glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), (int)value); 
    }
    void citro_shader::setInt(const std::string &name, int value) const{
        //glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), value); 
    }
    void citro_shader::setFloat(const std::string &name, float value) const{
        //glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value); 
    }
    citro_screen::citro_screen() noexcept{
        C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
        target = 
        C3D_RenderTargetCreate(
            240, 400, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
        C3D_RenderTargetSetOutput(
            target, GFX_TOP, GFX_LEFT, DISPLAY_TRANSFER_FLAGS);
    };

    citro_screen::~citro_screen() noexcept{
        C3D_Fini();
    }

    bool citro_screen::isAppRunning() const noexcept{
        return (aptMainLoop() && running);
    }

    void citro_screen::closeApp() noexcept{
        running = false;
    }

    bool citro_screen::getInputPressed(KeyInput input) const noexcept{
        bool devolver = false;
        hidScanInput();
        u32 kDown = hidKeysDown();
        switch (input){
            case KeyInput::exit :
                if((kDown & KEY_START))
                    devolver = true;
            break;
            default: 
            break;
        }
        return devolver;
    }

    constexpr int BACKGROUND_COLOR = 0x334D4DFF;

    void citro_screen::swapBuffers() const noexcept{
        C3D_FrameEnd(0);
    };

    void citro_screen::prepareDraw() const noexcept{
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C3D_RenderTargetClear(target, C3D_CLEAR_ALL, BACKGROUND_COLOR, 0);
		C3D_FrameDrawOn(target);
    }

    citro_main::citro_main() noexcept{
        Result rc = romfsInit();
        if(rc)
            printf("romfsInit: %08lX\n", rc);
        else
            printf("romfs success\n");
        gfxInitDefault();
        consoleInit(GFX_BOTTOM, NULL);

    };

    citro_main::~citro_main() noexcept{
        gfxExit();
        romfsExit();
    }

}
//#endif
