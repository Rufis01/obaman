#ifndef MODEL_H
#define MODEL_H
#include <stdbool.h>

typedef struct i_R_Model R_Model;

R_Model* R_LoadModel(const char* path);
bool R_UnloadModel(R_Model *mod);

#endif
