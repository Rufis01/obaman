#ifndef INPUT_H
#define INPUT_H

#define BUTTON_ACTION 0
#define BUTTON_MENU 1

#define MAX_MOVEMENT 127

#define I_NO_DEVICE -1

typedef enum
{
	I_PORT_DEFAULT
} I_Port;

typedef struct
{
	int x;
	int y;

	int buttons;
} I_InputState;

typedef enum
{
	I_PAD_SELECT		= 1 << 0,
	I_PAD_L3			= 1 << 1,
	I_PAD_R3			= 1 << 2,
	I_PAD_START		= 1 << 3,
	I_PAD_UP			= 1 << 4,
	I_PAD_RIGHT		= 1 << 5,
	I_PAD_DOWN		= 1 << 6,
	I_PAD_LEFT		= 1 << 7,
	I_PAD_L2			= 1 << 8,
	I_PAD_R2			= 1 << 9,
	I_PAD_L1			= 1 << 10,
	I_PAD_R1			= 1 << 11,
	I_PAD_TRIANGLE	= 1 << 12,
	I_PAD_CIRCLE		= 1 << 13,
	I_PAD_CROSS		= 1 << 14,
	I_PAD_SQUARE		= 1 << 15,
} I_Button;

void I_Init();
int I_Poll(I_InputState *state, I_Port port);

#endif
