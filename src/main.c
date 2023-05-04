#include "definitions.h"

#include "math/real.h"

int main(void)
{
	real a = to_real(1.67);
	int b = 1;
	R_Init();
	FS_Init();
	I_Init();
	S_Init();
	SYS_Init();

	//a = sum(b,b);
	a = sum(a,b);	//2.67
	a = sum(b,a);	//3.67
	a = sum(a,a);	//7.34

	print(a);
	/*print(to_real(0.00015));
	print(to_real(0.0002));
	print(to_real(0.0015));
	print(to_real(0.002));
	print(to_real(0.015));
	print(to_real(0.02));
	print(to_real(0.15));
	print(to_real(0.2));
	print(to_real(1.5));
	print(to_real(2.0));*/
	//G_Start();
	
	return 0;
}
