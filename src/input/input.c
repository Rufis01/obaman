#include "input.h"
#include <psxpad.h>
#include <psxapi.h>

char pad_buff[2][0x22];	//BIOS needs 0x22 bytes, we might need less

void i_I_ReadDigitalPad(I_InputState *state, PADTYPE *pad);
void i_I_ReadAnalogPad(I_InputState *state, PADTYPE *pad);

void I_Init()
{
	// Init BIOS pad driver and set pad buffers (buffers are updated
	// automatically on every V-Blank)
	InitPAD(&pad_buff[0][0], 34, &pad_buff[1][0], 34);
	
	// Start pad
	StartPAD();
	
	// Don't make pad driver acknowledge V-Blank IRQ (recommended)
	ChangeClearPAD(0);


	printf("Input initialized!\n");
}

int I_Poll(I_InputState *state, I_Port port)
{
	PADTYPE *pad = (PADTYPE *) pad_buff[port];
	switch(pad->type)
	{
		case PAD_ID_DIGITAL:
			i_I_ReadDigitalPad(state, pad);
			break;
		case PAD_ID_ANALOG:
			i_I_ReadAnalogPad(state, pad);
			break;
		default:
			return I_NO_DEVICE;
	}
	return 0;
}

void i_I_ReadDigitalPad(I_InputState *state, PADTYPE *pad)
{
	state->buttons = ~pad->btn;

	state->y = state->x = 0;

	if(state->buttons & PAD_UP) state->y = MAX_MOVEMENT;
	else if (state->buttons & PAD_DOWN) state->y = -MAX_MOVEMENT;

	if(state->buttons & PAD_RIGHT) state->x = MAX_MOVEMENT;
	else if (state->buttons & PAD_LEFT) state->x = -MAX_MOVEMENT;
}

void i_I_ReadAnalogPad(I_InputState *state, PADTYPE *pad)
{
	i_I_ReadDigitalPad(state, pad);
}
