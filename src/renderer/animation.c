#include <stdint.h>

#include "animation.h"
#include "i_model.h"

///NOTE: this is kinda shitty, no?
struct i_R_Point
{
	uint16_t index;
	i_R_Vertex vertex;
};

struct i_R_Anim
{
	uint32_t duration;
	R_EasingFunction easing:8;
	uint8_t flags;
	uint16_t points;
	struct i_R_Point *data;

	R_Model *target;
	i_R_Vertex *startData;		//We need a copy of the original vertices to do LERP
	uint32_t startTime;

};

R_Anim *R_LoadAnim(const char *path, R_Model *target)
{

	R_Anim *out = malloc(sizeof(R_Anim));
	if(!out)
		return 0;

	char *animData = FS_LoadFileSync(path, 0);
	char *ptr = animData;

	if(!ptr)
	{
		free(out);
		return 0;
	}

	out->target = target;

	///NOTE: This can be optimized
	unsigned int sz;

	sz = 4;
	memcpy(out->duration, ptr, sz);
	ptr += sz;

	sz = 1;
	memcpy(out->easing, ptr, sz);
	ptr += sz;

	sz = 1;
	memcpy(out->flags, ptr, sz);
	ptr += sz;

	sz = 2;
	memcpy(out->points, ptr, sz);
	ptr += sz;
	
	
	out->data = malloc(sizeof(struct i_R_Point) * out->points);
	out->startData = malloc(sizeof(i_R_Vertex) * out->points);

	if(!out->data || !out->startData)
	{
		free(out->data);
		free(out->startData);
		free(animData);
		free(out);
		return 0;
	}

	sz = sizeof(struct i_R_Point) * out->points;
	memcpy(out->data, ptr, sz);

	///NOTE: possibly very slow
	for(int i = 0; i<out->points; i++)
	{
		memcpy(&out->startData[i], &out->data[i].vertex, sizeof(i_R_Vertex));
	}
	
	free(animData);

	return out;
}

void R_UpdateAnim(R_Anim *anim, int dt)
{
//https://www.desmos.com/calculator/eq37cxi1vb
}

bool R_UnloadAnim(R_Anim *anim)
{
	free(anim->data);
	free(anim);
}