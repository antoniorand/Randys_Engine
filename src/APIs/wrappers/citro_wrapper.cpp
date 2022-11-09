#ifdef __3DS__
//Source: https://github.com/tinyobjloader/tinyobjloader
#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
#include "../../dependencies/tiny_obj_loader.h"

#include "citro_wrapper.hpp"
namespace RandysEngine{

    #define DISPLAY_TRANSFER_FLAGS \
	(GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(0) | GX_TRANSFER_RAW_COPY(0) | \
	GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8) | \
	GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO))


    void citro_skybox_helper::reverseCull() const noexcept{
        C3D_CullFace(GPU_CULL_FRONT_CCW);
    }

    void citro_skybox_helper::restoreCull() const noexcept{
        C3D_CullFace(GPU_CULL_BACK_CCW);
    }

    const C3D_Mtx& citro_matrix::getTransformationMatrix() noexcept{
        if(changed){

            if(perspective){
                Mtx_PerspTilt(&transform, C3D_AngleFromDegrees(fov), C3D_AspectRatioTop, near, far, true);

            }
            else{
                Mtx_Identity(&transform);
            }
            
            Mtx_Translate(&transform, translation[0],
                translation[1],translation[2], true);
            Mtx_RotateX(&transform, rotation[0], true);
            Mtx_RotateY(&transform, rotation[1], true);
            Mtx_RotateZ(&transform, rotation[2], true);
            Mtx_Scale(&transform,scalation[0],scalation[1],scalation[2]);
            changed = false;
        }
        return transform;
    }
    
    void citro_matrix::multiply(citro_matrix& other){
        this->changed = true;
        auto thisMatrix = this->getTransformationMatrix();
        auto otherMatrix = other.getTransformationMatrix();

        Mtx_Multiply(&transform,&otherMatrix,&thisMatrix);

    }

    citro_matrix citro_matrix::reverse() noexcept{
        citro_matrix devolver;
        devolver.changed = false;
        devolver.transform = this->getTransformationMatrix();
        Mtx_Inverse(&devolver.transform);

        return(devolver);
    }

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
        return devolver;
    }

    citro_texture_resource::citro_texture_resource(std::string file) noexcept{
        // Load the texture and bind it to the first texture unit
	    if (!loadTextureFromFile(&texture, NULL, file.c_str()))
	    	svcBreak(USERBREAK_PANIC);
        C3D_TexSetWrap(&texture, GPU_REPEAT, GPU_REPEAT);
        C3D_TexSetFilter(&texture, GPU_LINEAR, GPU_LINEAR);
    }

    void citro_texture_resource::setToSkybox() noexcept{
        C3D_TexSetWrap(&texture, GPU_CLAMP_TO_EDGE, GPU_CLAMP_TO_EDGE);
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

    std::vector<Vertex> citro_mesh_resource::loadModel(std::string file) noexcept{

        std::vector<Vertex> devolver;

        tinyobj::ObjReaderConfig reader_config;
        reader_config.mtl_search_path = "./"; // Path to material files

        tinyobj::ObjReader reader;

        if (!reader.ParseFromFile(file, reader_config)) {
            if (!reader.Error().empty()) {
                //std::cerr << "TinyObjReader: " << reader.Error();
            }
        }

        if (!reader.Warning().empty()) {
            //std::cout << "TinyObjReader: " << reader.Warning();
        }

        auto& attrib = reader.GetAttrib();
        auto& shapes = reader.GetShapes();

        devolver.reserve(attrib.vertices.size());

        // Loop over shapes
        for (size_t s = 0; s < shapes.size(); s++) {
            // Loop over faces(polygon)
            size_t index_offset = 0;
            for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
                size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

                // Loop over vertices in the face.
                for (size_t v = 0; v < fv; v++) {
                    // access to vertex
                    tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                    tinyobj::real_t vx = attrib.vertices[3*size_t(idx.vertex_index)+0];
                    tinyobj::real_t vy = attrib.vertices[3*size_t(idx.vertex_index)+1];
                    tinyobj::real_t vz = attrib.vertices[3*size_t(idx.vertex_index)+2];

                    // Check if `normal_index` is zero or positive. negative = no normal data
                    /*if (idx.normal_index >= 0) {
                        tinyobj::real_t nx = attrib.normals[3*size_t(idx.normal_index)+0];
                        tinyobj::real_t ny = attrib.normals[3*size_t(idx.normal_index)+1];
                        tinyobj::real_t nz = attrib.normals[3*size_t(idx.normal_index)+2];
                    }*/

                    tinyobj::real_t tx = 0;
                    tinyobj::real_t ty = 0;
                    // Check if `texcoord_index` is zero or positive. negative = no texcoord data

                    if (idx.texcoord_index >= 0) {
                        tx = attrib.texcoords[2*size_t(idx.texcoord_index)+0];
                        ty = attrib.texcoords[2*size_t(idx.texcoord_index)+1];
                    }

                    devolver.emplace_back(vx,vy,vz,tx,ty);

                    // Optional: vertex colors
                    // tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
                    // tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
                    // tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];
                }
                index_offset += fv;
            }

        }

        count_loadedVertices = devolver.size();
        size_loadedVertices = count_loadedVertices*sizeof(Vertex);

        return devolver;

    }

    citro_mesh_resource::citro_mesh_resource(std::string input) noexcept{
        std::vector<Vertex> convertedVertices = loadModel(input);

        vbo_data = linearAlloc(size_loadedVertices);
        memcpy(vbo_data,&convertedVertices[0], size_loadedVertices);

    }

    citro_mesh_resource::~citro_mesh_resource() noexcept{
        linearFree(vbo_data);
    }

    void citro_mesh_resource::draw() const noexcept{

        // Configure buffers
        C3D_BufInfo* bufInfo = C3D_GetBufInfo();
        BufInfo_Init(bufInfo);
        BufInfo_Add(bufInfo, vbo_data, sizeof(Vertex), 2, 0x10);

        // Draw the VBO
        C3D_DrawArrays(GPU_TRIANGLES, 0, count_loadedVertices);

    }

    citro_shader::citro_shader() noexcept{
        // Load the vertex shader, create a shader program and bind it
        vshader_dvlb = DVLB_ParseFile((u32*)vshader_shbin,
        vshader_shbin_size);
        shaderProgramInit(&shaderProgram);
        shaderProgramSetVsh(&shaderProgram, &vshader_dvlb->DVLE[0]);
        C3D_BindProgram(&shaderProgram);
        
        // Configure attributes for use with the vertex shader
        C3D_AttrInfo* attrInfo = C3D_GetAttrInfo();
        AttrInfo_Init(attrInfo);
        AttrInfo_AddLoader(attrInfo, 0, GPU_FLOAT, 3); // v0=position
        AttrInfo_AddLoader(attrInfo, 1, GPU_FLOAT, 2); // v1=position

        
        // Configure the first fragment shading substage to just pass through the vertex color
        // See https://www.opengl.org/sdk/docs/man2/xhtml/glTexEnv.xml for more insight
        C3D_TexEnv* env = C3D_GetTexEnv(0);
        C3D_TexEnvInit(env);

        C3D_TexEnvSrc(env, C3D_Both, GPU_TEXTURE0, (GPU_TEVSRC)0, (GPU_TEVSRC)0);
	    C3D_TexEnvFunc(env, C3D_Both, GPU_MODULATE);
    }

    citro_shader::~citro_shader() noexcept{
        shaderProgramFree(&shaderProgram);
        DVLB_Free(vshader_dvlb);
    }

    void citro_shader::useShader() noexcept{
        C3D_BindProgram(&shaderProgram);
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

    void citro_shader::setMat4(const std::string &name, citro_matrix &mat){
        int uLocation = 
            shaderInstanceGetUniformLocation(shaderProgram.vertexShader, name.c_str());
        C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uLocation,  &mat.getTransformationMatrix());
    }

    citro_screen::citro_screen() noexcept{
        C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
        target = 
        C3D_RenderTargetCreate(
            240, 400, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
        C3D_RenderTargetSetOutput(
            target, GFX_TOP, GFX_LEFT, DISPLAY_TRANSFER_FLAGS);
        C3D_CullFace(GPU_CULL_BACK_CCW);
        C3D_DepthTest(true, GPU_GREATER, GPU_WRITE_ALL);
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
        u32 kDown = hidKeysHeld();
        switch (input){
            case KeyInput::exit :
                if((kDown & KEY_START))
                    devolver = true;
            break;
            case KeyInput::left :
                if((kDown & KEY_LEFT))
                    devolver = true;
            break;
            case KeyInput::right :
                if((kDown & KEY_RIGHT))
                    devolver = true;
            break;
            case KeyInput::up :
                if((kDown & KEY_UP))
                    devolver = true;
            break;
            case KeyInput::down :
                if((kDown & KEY_DOWN))
                    devolver = true;
            break;
            case KeyInput::a_button:
                if((kDown & KEY_A))
                    devolver = true;
            break;
            case KeyInput::b_button:
                if((kDown & KEY_B))
                    devolver = true;
            break;
            case KeyInput::x_button:
                if((kDown & KEY_X))
                    devolver = true;
            break;
            case KeyInput::y_button:
                if((kDown & KEY_Y))
                    devolver = true;
            break;
            default: 
            break;
        }
        return devolver;
    }

    constexpr int BACKGROUND_COLOR = 0x334D4DFF;
    constexpr int DEPTH_VALUE = 0x00000000;

    void citro_screen::swapBuffers() const noexcept{
        C3D_FrameEnd(0);
    };

    void citro_screen::clearDepth() const noexcept{
        C3D_RenderTargetClear(target, C3D_CLEAR_DEPTH, 0x00000000, DEPTH_VALUE);
    }

    void citro_screen::prepareDraw() const noexcept{
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C3D_RenderTargetClear(target, C3D_CLEAR_ALL, BACKGROUND_COLOR, DEPTH_VALUE);
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
#endif
