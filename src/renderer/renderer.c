#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include <psxgpu.h>
#include <psxgte.h>
#include <psxetc.h>
#include <inline_c.h>

#include "renderer.h"
#include "i_model.h"

#define OT_LEN 4096
#define WIDTH 640
#define HEIGHT 512

#ifdef __INTELLISENSE__
    #pragma diag_suppress 1118
#endif

struct BUFFER
{
	DISPENV disp;
	DRAWENV draw;
	uint32_t table[OT_LEN];
	uint8_t buffer[32768 * 2];
} screenBuf[2];

char sb;
uint8_t *nextprim;

/* Light color matrix */
/* Each column represents the color matrix of each light source and is */
/* used as material color when using gte_ncs() or multiplied by a */
/* source color when using gte_nccs(). 4096 is 1.0 in this matrix */
/* A column of zeroes disables the light source. */
MATRIX color_mtx = {
	ONE/2, 0, 0,	/* Red   */
	ONE/2, 0, 0,	/* Green */
	ONE/2, 0, 0	/* Blue  */
};

/* Light matrix */
/* Each row represents a vector direction of each light source. */
/* An entire row of zeroes disables the light source. */
MATRIX light_mtx = {
	/* X,  Y,  Z */
	-ONE , -ONE , -ONE,
	0	  , 0	  , 0,
	0	  , 0	  , 0
};

void R_SetFov(int tan)
{
	gte_SetGeomScreen(tan);
}

void R_Init()
{
	// This not only resets the GPU but it also installs the library's
	// ISR subsystem to the kernel
	ResetGraph(0);
	
	// Define display environments, first on top and second on bottom
	SetDefDispEnv(&screenBuf[0].disp, 0, 0, WIDTH, HEIGHT);
	SetDefDispEnv(&screenBuf[1].disp, 0, 0, WIDTH, HEIGHT);
	screenBuf[0].disp.isinter = 1;
	screenBuf[1].disp.isinter = 1;

    // Configures the pair of DRAWENVs for the DISPENVs
    SetDefDrawEnv(&screenBuf[0].draw, 0, 0, WIDTH, HEIGHT);
    SetDefDrawEnv(&screenBuf[1].draw, 0, 0, WIDTH, HEIGHT);

	// Set and enable clear color
	setRGB0(&screenBuf[0].draw, 0, 15, 63);
	setRGB0(&screenBuf[1].draw, 0, 15, 63);
	screenBuf[0].draw.isbg = 1;
	screenBuf[1].draw.isbg = 1;
	
	// Apply the GPU environments
	PutDispEnv(&screenBuf[0].disp);
	PutDrawEnv(&screenBuf[0].draw);

	FntLoad(960, 0);
	FntOpen(0, 16, WIDTH, HEIGHT, 1, 100);
	
	// Enable display output, ResetGraph() disables it by default
	SetDispMask(1);

	sb = 0;
	nextprim = screenBuf[0].buffer;

	InitGeom();

	gte_SetGeomOffset( WIDTH/2, HEIGHT/2 );
	
	/* Set screen depth (basically FOV control) */
	// = WIDTH * tan(FOV/2)
	gte_SetGeomScreen( WIDTH/2 );
	
	/* Set light ambient color and light color matrix */
	gte_SetBackColor( 31, 31, 31 );
	gte_SetColorMatrix( &color_mtx );
}

void i_R_GetWorldMatirx(G_Camera *cam, MATRIX* wmtx)
{
	VECTOR	ipos;	// Integer part of cam position
	SVECTOR	irot;	// Integer part of cam rotation

	ipos.vx = -(cam->position.vx >> 12);
	ipos.vy = -(cam->position.vy >> 12);
	ipos.vz = -(cam->position.vz >> 12);
	
	irot.vx = cam->rotation.vx;
	irot.vy = cam->rotation.vy;
	irot.vz = cam->rotation.vz;

	RotMatrix(&irot, wmtx);				//Apply rotation to the matrix
	ApplyMatrixLV(wmtx, &ipos, &ipos);	//Apply rotation to the translation vector
	TransMatrix(wmtx, &ipos);			//Apply translation to the matrix

	gte_SetRotMatrix(wmtx);
	gte_SetTransMatrix(wmtx);
}

void R_DrawEntity(G_Camera *cam, G_Entity *ent)
{
	///NOTE: Can be optimized(?) by calculating world matrix once and applying it to multiple entities
	R_Model *model = ent->model;
	VECTOR	pos;
	SVECTOR	*rot = &ent->rotation;
	MATRIX	wmtx, omtx, lmtx;		//World, object and light matrices
	
	pos.vx = ent->position.vx >> 12;
	pos.vy = ent->position.vy >> 12;
	pos.vz = ent->position.vz >> 12;

	i_R_GetWorldMatirx(cam, &wmtx);

	// Rotate and translate MV matrix
	RotMatrix( rot, &omtx );
	TransMatrix( &omtx, &pos );

	// Apply the same transformation to the light matrix
	// so the light source won't be relative to the model
	MulMatrix0( &light_mtx, &omtx, &lmtx );

	// Composite coordinate matrix transform, so object will be rotated and
	// positioned relative to camera matrix (wmtx), so it'll appear as
	// world-space relative.
	CompMatrixLV( &wmtx, &omtx, &omtx );

	// 
	gte_SetRotMatrix( &omtx );
	gte_SetTransMatrix( &omtx );

	// 
	gte_SetLightMatrix( &lmtx );

	//Calculate polygons' projections and draw them to screen

	POLY_G3 *pol3 = (POLY_G3*) nextprim;

	//printf("Indices: %d, Vertices: %d\n",model->indices,model->vertices);

	for(int i = 0; i < model->indices; i+=3)
	{
		if((char *)pol3 > (char *)&screenBuf[sb].buffer[32768*2])
		{
			printf("GPU primitive buffer is full!\n");
			return;
		}

		// Load 3 vectors (polygon's vertices) to the GTE
		unsigned short index0 = model->indexData[i+2];
		unsigned short index1 = model->indexData[i+1];
		unsigned short index2 = model->indexData[i+0];
		gte_ldv3(
				&model->vertexData[index0].position,
				&model->vertexData[index1].position,
				&model->vertexData[index2].position );

		// Rotate, transform and calculate prospective of 3 vertices
		gte_rtpt();

		// Compute normal clipping
		gte_nclip();

		// Get result from GTE register C2_MAC0
		// gte_stopz is undocumented
		// If p < 0 the polygon is facing away
		// from the camera and doesn't need to be drawn
		int p;
		gte_stopz(&p);

		if(p < 0) continue;

		// Calculate avarage z of the polygon,
		// scale it down and skip if clipping (too far from camera)
		gte_avsz3();
		gte_stotz(&p);

		p >>= 4;

		if( (p <= 0) || (p > OT_LEN) )
			{ printf("Toofarfromcam\n"); continue; }

		// Now the polygon is ready to be sent to the GPU
		// so we assemble a packet

		setPolyG3(pol3);

		// We retrieve the coordinates from the coprocessor
		gte_stsxy0(&pol3->x0);
		gte_stsxy1(&pol3->x1);
		gte_stsxy2(&pol3->x2);

		/* Load primitive color even though gte_nct() doesn't use it. */
		/* This is so the GTE will output a color result with the */
		/* correct primitive code. */
		gte_ldrgb( &pol3->r0 );

		// Load the normal for light calculation
		gte_ldv3(
				&model->vertexData[index0].normal,
				&model->vertexData[index1].normal,
				&model->vertexData[index2].normal );

		// Calculate color based on normal
		gte_nct();

		// Retrieve the result
		gte_strgb3(&pol3->r0, &pol3->r1, &pol3->r2);
		// Order the primitive
		addPrim(screenBuf[sb].table + p, pol3);

		pol3++;
	}
	nextprim = (char*) pol3;
}

void R_Finish()
{
	FntFlush(-1);

	// Wait for all drawing to complete
	DrawSync(0);
	
	// Wait for vertical sync to cap the logic to 60fps (or 50 in PAL mode)
	// and prevent screen tearing
	VSync(0);

	// Switch pages	
	PutDispEnv(&screenBuf[sb].disp);
	PutDrawEnv(&screenBuf[sb].draw);

	// Draw the Ordering Table
	// It's a Reverse OT, so we start from the bottom
	DrawOTag((u_long*)(screenBuf[sb].table+OT_LEN-1));

	// Flip buffer index
	
	sb ^= 1;
	ClearOTagR((u_long*) screenBuf[sb].table, OT_LEN);
	nextprim = screenBuf[sb].buffer;
}
