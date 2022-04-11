#ifndef I_MODEL_H
#define I_MODEL_H
#include <psxgte.h>

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

    i_R_Vertex* vertexData;
    unsigned short* indexData;
};

#endif