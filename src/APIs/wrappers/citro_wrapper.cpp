
#ifdef __3DS__
#include "citro_wrapper.hpp"
namespace RandysEngine{

    #define DISPLAY_TRANSFER_FLAGS \
	(GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(0) | GX_TRANSFER_RAW_COPY(0) | \
	GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8) | \
	GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO))

    Vertex verticesConverter(Vertex vertex){
        RandysEngine::Vertex devolver;     
        devolver.x = 200*vertex.x + 200;    
        devolver.y = 120*vertex.y + 120;
        devolver.z = 0.5;    
        return devolver;
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

        // Configure buffers
        C3D_BufInfo* bufInfo = C3D_GetBufInfo();
        BufInfo_Init(bufInfo);
        BufInfo_Add(bufInfo, vbo_data, sizeof(Vertex), 1, 0x0);
    }

    citro_mesh_resource::~citro_mesh_resource() noexcept{
        linearFree(vbo_data);
        linearFree(ibo_data);
    }

    void citro_mesh_resource::draw() const noexcept{
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
        AttrInfo_AddFixed(attrInfo, 1); // v1=color

        // Set the fixed attribute (color) to selected one
        C3D_FixedAttribSet(1, 1.0, 0.5, 0.2, 1.0);

        // Compute the projection matrix
        Mtx_OrthoTilt(&projection, 0.0, 400.0, 0.0, 240.0, 0.0, 1.0, true);
        
        // Configure the first fragment shading substage to just pass through the vertex color
        // See https://www.opengl.org/sdk/docs/man2/xhtml/glTexEnv.xml for more insight
        C3D_TexEnv* env = C3D_GetTexEnv(0);
        C3D_TexEnvInit(env);
        C3D_TexEnvSrc(env, C3D_Both, GPU_PRIMARY_COLOR, (GPU_TEVSRC)0, (GPU_TEVSRC)0);
        C3D_TexEnvFunc(env, C3D_Both, GPU_REPLACE);
    }

    citro_shader::~citro_shader() noexcept{
        shaderProgramFree(&shaderProgram);
        DVLB_Free(vshader_dvlb);
    }

    void citro_shader::useShader() const noexcept{
        // Update the uniforms
    	C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uLoc_projection, &projection);
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

        gfxInitDefault();
        consoleInit(GFX_BOTTOM, NULL);

    };

    citro_main::~citro_main() noexcept{
        gfxExit();
    }

}
#endif
