#ifndef CAMERA_H
#define CAMERA_H

#include <psxgte.h>

typedef struct
{
	VECTOR position;
	SVECTOR rotation;
	int FOV;			//Actually (tan(FOV/2))
} G_Camera;

#endif
