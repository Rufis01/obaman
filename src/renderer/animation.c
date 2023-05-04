#include <stdint.h>

#include "animation.h"
#include "i_model.h"

///NOTE: this is kinda shitty, no?
//But we only store the points that changed, so it's fine?
struct i_R_Point
{
	uint16_t index;
	i_R_Vertex vertex_end;
	i_R_Vertex vertex_start;	//We need a copy of the original vertices to do LERP
};

struct i_R_Anim
{
	uint32_t duration;
	R_EasingFunction easing:8;
	uint8_t flags;
	uint16_t points;
	struct i_R_Point *data;

	R_Model *target;
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

	out->startTime = 0;

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

	if(!out->data)
	{
		free(animData);
		free(out);
		return 0;
	}

	for(int i = 0; i<out->points; i++)
	{
		sz = sizeof(struct i_R_Point) - sizeof(i_R_Vertex);
		memcpy(&out->data[i], ptr, sz);
		ptr+=sz;
	}

	for(int i = 0; i<out->points; i++)
	{
		uint16_t idx = out->data[i].index;
		memcpy(&out->data[i].vertex_start, &out->target->vertexData[idx], sizeof(i_R_Vertex));
	}
	
	free(animData);

	return out;
}

///TODO: FIXED POINT MATH IS A DICK

short SLERP(short x1, short x2, short t)
{
	return x1 + t * (x2 - x1);
}

void SVLERP(SVECTOR *out, SVECTOR *v1, SVECTOR *v2, short t)
{
	out->vx = SLERP(v1->vx, v2->vx, t);
	out->vy = SLERP(v1->vy, v2->vy, t);
	out->vz = SLERP(v1->vz, v2->vz, t);
}

void DVLERP(DVECTOR *out, DVECTOR *v1, DVECTOR *v2, short t)
{
  out->vx = SLERP(v1->vx, v2->vx, t);
  out->vy = SLERP(v1->vy, v2->vy, t);
}

void R_UpdateAnim(R_Anim *anim, int dt)
{
	if(anim->startTime == 0)
		anim->startTime = S_GetTicks();
	
	for(int i=0; i< anim->points; i++)
	{
		uint16_t idx = anim->data[i].index;
		i_R_Vertex *vtx_out = &anim->target->vertexData[idx];
		i_R_Vertex *vtx1 = &anim->data[i].vertex_start;
		i_R_Vertex *vtx2 = &anim->data[i].vertex_end;

		SVLERP(&vtx_out->normal, &vtx1->normal, &vtx2->normal, dt);
		SVLERP(&vtx_out->position, &vtx1->position, &vtx2->position, dt);
		DVLERP(&vtx_out->uv, &vtx1->uv, &vtx2->uv, dt);
	}
//https://www.desmos.com/calculator/eq37cxi1vb
}

bool R_UnloadAnim(R_Anim *anim)
{
	free(anim->data);
	free(anim);
}