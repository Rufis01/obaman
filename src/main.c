#include "definitions.h"

int main(void)
{
	R_Init();
	FS_Init();
	I_Init();
	S_Init();

	G_Start();
	
	return 0;
}
