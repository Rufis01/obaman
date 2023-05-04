/*
This library is intended for interoperation between floating point and fixed point numbers.
This engine is intended to run on a big variety of systems and not all systems have floating point capabilities.
Furthermore this library will simplify aritmethical operations in the case of fixed point numbers, by taking care of all the necessary steps to multiply and divide fixed point numbers.
*/

#ifndef REAL_H
#define REAL_H

#ifdef USE_FLOAT

typedef float real;
typedef double dpreal;

#else

#include <stdint.h>

#define FRACTION_BITS (12)
static const int one = (1<<FRACTION_BITS);
static const int FRACTION_MASK = one-1;

//typedef struct{int16_t v;} sreal;
typedef struct{int32_t v;} real;
typedef struct{int64_t v;} dpreal;

#define REAL(x) ((real){(x)})

#endif

//real to_real(int n);
real to_real(float n);
int to_int(real n);
int fraction_of(real n);


real sum_ii(int n1, int n2);
real sum_ri(real n1, int n2);
real sum_ir(int n1, real n2);
real sum_rr(real n1, real n2);
/*
int  sub_ii(int n1, int n2);
real sub_ri(real n1, int n2);
real sub_ir(int n1, real n2);
real sub_rr(real n1, real n2);

int  mul_ii(int n1, int n2);
real mul_ri(real n1, int n2);
real mul_ir(int n1, real n2);
real mul_rr(real n1, real n2);

int  div_ii(int n1, int n2);
real div_ri(real n1, int n2);
real div_ir(int n1, real n2);
real div_rr(real n1, real n2);
*/

#define sum(n1, n2) _Generic((n1),\
							  int: _Generic((n2),\
							  				int: sum_ii,\
											real: sum_ir\
										   ),\
							  real: _Generic((n2),\
							  				int: sum_ri,\
											real: sum_rr\
										   )\
							)(n1, n2);

#endif