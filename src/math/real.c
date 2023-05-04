#include "real.h"

/*real to_real(int n)
{
	return n<<FRACTION_BITS;
}

real to_real(int integer, int fraction)	//NOT YET IMPLEMENTED
{
	printf("WARNING real to_real(int, int) has not been implemented yet");
	return 0;
}*/

real to_real(float n) 
{
	return REAL((int)(n*one));
}

int to_int(real n)
{
	return n.v>>FRACTION_BITS;
}

int fraction_of(real n)
{
	//This ensures we get maximum precision for a base 10 representation.
	return ((n.v & FRACTION_MASK) * 10000) >> FRACTION_BITS;
}

void print(real n)
{
	printf("%d.%04d\n", to_int(n), fraction_of(n));
}

//SUM

real sum_ii(int n1, int n2)
{
	return REAL((n1+n2) << FRACTION_BITS);
}
real sum_ri(real n1, int n2)
{
	return REAL(n1.v + (n2<<FRACTION_BITS));
}
real sum_ir(int n1, real n2)
{
	return REAL(n2.v + (n1<<FRACTION_BITS));
}
real sum_rr(real n1, real n2)
{
	return REAL(n1.v + n2.v);
}
/*
//SUB

int sub_ii(int n1, int n2)
{
	return n1-n2;
}
real sub_ri(real n1, int n2)
{
	return n1 - (n2<<FRACTION_BITS);
}
real sub_ir(int n1, real n2)
{
	return -n2 + (n1<<FRACTION_BITS);
}
real sub_rr(real n1, real n2)
{
	return n1-n2;
}

//MUL

int mul_ii(int n1, int n2)
{
	return (n1*n2) << FRACTION_BITS;
}
real mul_ri(real n1, int n2)
{
	return n1 * n2;
}
real mul_ir(int n1, real n2)
{
	return n1 * n2;
}
real mul_rr(real n1, real n2)
{
	return (((dpreal)(n1))*((dpreal)(n1))) >> FRACTION_BITS;
}
*/