#include <stdlib.h>

#include <psxgte.h>
#include <string.h>

#include "model.h"
#include "i_model.h"
#include "../filesystem/filesystem.h"

R_Model *R_LoadModel(const char *path)
{
	R_Model *out = malloc(sizeof(R_Model));
	if(!out)
		return 0;

	///TODO: maybe check actual vs expected size?
	char *modData = FS_LoadFileSync(path, 0);
	char *ptr = modData;

	if(!ptr)
	{
		free(out);
		return 0;
	}
	
	// Copies the first 2 fields of the model and advances the pointer
	memcpy(out, ptr, 2*sizeof(unsigned short));
	ptr +=  2*sizeof(unsigned short);
	
	out->vertexData = malloc(out->vertices * sizeof(i_R_Vertex));
	out->indexData = malloc(out->indices * sizeof(unsigned short));

	if(!(
		out->vertexData &&
		out->indexData ))
	{
		free(modData);
		free(out->vertexData);
		free(out->indexData);
		free(out);
		return 0;
	}

	unsigned int sz;

	sz = out->vertices * sizeof(i_R_Vertex);
	memcpy(out->vertexData, ptr, sz);
	ptr += sz;

	sz = out->indices * sizeof(unsigned int);
	memcpy(out->indexData, ptr, sz);
	
	free(modData);

	return out;
}

bool R_UnloadModel(R_Model *mod)
{
	free(mod->vertexData);
	free(mod->indexData);

	free(mod);

	return true;
}
