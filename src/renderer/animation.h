#ifndef ANIMATION_H
#define ANIMATION_H

#include <stdbool.h>

#include "model.h"

typedef struct i_R_Anim R_Anim;

typedef enum
{
	EASING_NONE = 0,
	EASING_LINEAR,
	EASING_QUADRATIC,
	EASING_SINE,
	EASING_LOGARITHMIC,
	EASING_EXPONENTIAL,
	EASING_SPLINE	//NOT IMPLEMENTED
} R_EasingFunction;

enum R_Anim_Flags
{
	ANIMATION_LOOPS = 1 << 0,
	ANIMATION_REVERSE = 1 << 1,
};

R_Anim *R_LoadAnim(const char *path, R_Model *target);
void R_UpdateAnim(R_Anim *anim, int dt);
bool R_UnloadAnim(R_Anim *anim);

#endif