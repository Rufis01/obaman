#include "system.h"
#include <psxapi.h>
#include <hwregs_c.h>

volatile int timer_calls = 0;

void timer_func()
{
	timer_calls++;
}

void SYS_Init(void)
{
	EnterCriticalSection();
	int counter = (4233600 * 4) / 1000; //4000000 * 4 / 1000 for older consoles (?)
	
	SetRCnt(RCntCNT2, counter, RCntMdINTR);
	TIMER_CTRL(2) = 0x00FA;
	InterruptCallback(6, timer_func);
	StartRCnt(RCntCNT2);
	ChangeClearRCnt(2, 0);
	ExitCriticalSection();

	printf("System initialized!\n");
}

void SYS_GetTicks(void)
{

}

void SYS_Fini()
{

}