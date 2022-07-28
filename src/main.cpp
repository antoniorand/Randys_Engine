#include <3ds.h>
#include <citro3d.h>
#include <string.h>
#include "vshader_shbin.h"

#define CLEAR_COLOR 0x68B0D8FF

#define DISPLAY_TRANSFER_FLAGS \
	(GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(0) | GX_TRANSFER_RAW_COPY(0) | \
	GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8) | \
	GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO))

typedef struct { float x, y, z; } vertex;

static const vertex triangle1[] =
{
	{ 300.0f, 200.0f, 0.5f},
	{ 200.0f, 200.0f, 0.5f },
	{ 100.0f, 40.0f, 0.5f }
};


static const vertex triangle2[] =
{
	{ 0.0f, 0.0f, 0.5f},
	{ 10.0f, 10.0f, 0.5f },
	{ 0.0f, 20.0f, 0.5f }
};

#define vertex_list_count (sizeof(triangle1)/sizeof(triangle1[0]))

static DVLB_s* vshader_dvlb;
static shaderProgram_s program;
static int uLoc_projection;
static C3D_Mtx projection;

static void* vbo_data;
static void* vbo_data2;

static void sceneInit(void)
{
	// Load the vertex shader, create a shader program and bind it
	vshader_dvlb = DVLB_ParseFile((u32*)vshader_shbin, vshader_shbin_size);
	shaderProgramInit(&program);
	shaderProgramSetVsh(&program, &vshader_dvlb->DVLE[0]);
	C3D_BindProgram(&program);

	// Get the location of the uniforms
	uLoc_projection = shaderInstanceGetUniformLocation(program.vertexShader, "projection");

	// Configure attributes for use with the vertex shader
	// Obtain the global attribute info
	C3D_AttrInfo* attrInfo = C3D_GetAttrInfo();
	//Resets and initializes attribute info structure
	AttrInfo_Init(attrInfo);
	//Defines an array of vertex attribute data
	//Returns attribute index if successful
	//The attribute is defined in the shader
	AttrInfo_AddLoader(attrInfo, 0, GPU_FLOAT, 3); // v0=position
	//Defines a fixed vertex attribute
	//Returns attribute index if successful
	//The attribute is defined in the shader
	AttrInfo_AddFixed(attrInfo, 1); // v1=color

	// Set the fixed attribute (color) to solid white
	//It put the value '1' since is the '1' attribute ('0' is position)
	C3D_FixedAttribSet(1, 1.0, 1.0, 1.0, 1.0);

	// Compute the projection matrix
	Mtx_OrthoTilt(&projection, 0.0, 400.0, 0.0, 240.0, 0.0, 1.0, true);

	// Create the VBO (vertex buffer object)
	vbo_data = linearAlloc(sizeof(triangle1));
	memcpy(vbo_data, triangle1, sizeof(triangle1));

	// Create the VBO (vertex buffer object)
	vbo_data2 = linearAlloc(sizeof(triangle2));
	memcpy(vbo_data2, triangle2, sizeof(triangle2));

	// Configure the first fragment shading substage to just pass through the vertex color
	// See https://www.opengl.org/sdk/docs/man2/xhtml/glTexEnv.xml for more insight
	C3D_TexEnv* env = C3D_GetTexEnv(0);
	C3D_TexEnvInit(env);
	C3D_TexEnvSrc(env, C3D_Both, GPU_PRIMARY_COLOR, (GPU_TEVSRC)0, (GPU_TEVSRC)0);
	C3D_TexEnvFunc(env, C3D_Both, GPU_REPLACE);
}

static void sceneRender(void)
{
	// Update the uniforms
	C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uLoc_projection, &projection);

	// Configure buffers
	//Get the global info structure
	C3D_BufInfo* bufInfo = C3D_GetBufInfo();
	//Initializes it
	BufInfo_Init(bufInfo);
	//adds a buffer to the vertex info structure
	BufInfo_Add(bufInfo, vbo_data2, sizeof(vertex), 1, 0x0);

	// Draw the VBO
	C3D_DrawArrays(GPU_TRIANGLES, 0, vertex_list_count);

	//Initializes it
    C3D_BufInfo* bufInfo2 = C3D_GetBufInfo();
	BufInfo_Init(bufInfo2);
	//adds a buffer to the vertex info structure
	BufInfo_Add(bufInfo2, vbo_data, sizeof(vertex), 1, 0x0);

	// Draw the VBO
	C3D_DrawArrays(GPU_TRIANGLES, 0, vertex_list_count);
}

static void sceneExit(void)
{
	// Free the VBO
	linearFree(vbo_data);
    linearFree(vbo_data2);;
	// Free the shader program
	shaderProgramFree(&program);
	DVLB_Free(vshader_dvlb);
}

int main()
{
	// Initialize graphics
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);

	// Initialize the render target
	C3D_RenderTarget* target = C3D_RenderTargetCreate(240, 400, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
	C3D_RenderTargetSetOutput(target, GFX_TOP, GFX_LEFT, DISPLAY_TRANSFER_FLAGS);

	// Initialize the scene
	sceneInit();

	// Main loop
	while (aptMainLoop())
	{
		hidScanInput();

		// Respond to user input
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START)
			break; // break in order to return to hbmenu

		// Render the scene
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C3D_RenderTargetClear(target, C3D_CLEAR_ALL, CLEAR_COLOR, 0);
			C3D_FrameDrawOn(target);
			sceneRender();
		C3D_FrameEnd(0);
	}

	// Deinitialize the scene
	sceneExit();

	// Deinitialize graphics
	C3D_Fini();
	gfxExit();
	return 0;
}
