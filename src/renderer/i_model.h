#ifndef I_MODEL_H
#define I_MODEL_H
#include <psxgte.h>

///NOTE: typedef vs struct mismatch is awful, fix pls.

typedef struct
{
	SVECTOR position;
	DVECTOR uv;			//Maybe change this (UV can be at most a byte each, we can use the other 2 for something else)
	SVECTOR normal;
} i_R_Vertex;

struct i_R_Model
{
	unsigned short vertices;
	unsigned short indices;

	i_R_Vertex *vertexData;
	unsigned short *indexData;
};

//Single vertex with multiple attributes should be more cache-friendly
//but requires more types and could be less readable

//Indexed model should still be the way to go for now
//as it takes less memory and, even though it has less
//spatial locality, it should have more temporal locality

//As long as blender keeps vertex order constant when exporting to obj,
//morph target (per-vertex) animations should be trivial,
//even (especially?) with the indexed model;
//one just has to find those vertices which change and store the new location
//and interpolation data in some way.
//Other forms of animation will not be implemented for the time being.

struct R_Mono_Mesh
{
	SVECTOR *positions;
	SVECTOR *normals;
	CVECTOR color;
};

struct R_Color_Mesh
{
	SVECTOR *positions;
	SVECTOR *normals;
	CVECTOR *colors;
};

struct R_Tex_Mesh
{
	SVECTOR *positions;
	SVECTOR *normals;
	DVECTOR *uvs;
};

//Like Spyro?
struct R_ColorTex_Mesh
{
	SVECTOR *positions;
	SVECTOR *normals;
	DVECTOR *uvs;	
	CVECTOR *colors;
};

#endif
