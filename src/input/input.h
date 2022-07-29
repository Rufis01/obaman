#ifndef INPUT_H
#define INPUT_H

#define BUTTON_ACTION 0
#define BUTTON_MENU 1

#define MAX_MOVEMENT 127

#define I_NO_DEVICE -1

typedef enum
{
	INPUT_PORT_DEFAULT
} I_Port;

typedef struct
{
	int x;
	int y;

	unsigned int buttons;
} I_InputState;

typedef enum
{
	INPUT_PAD_SELECT		= 1 << 0,
	INPUT_PAD_L3			= 1 << 1,
	INPUT_PAD_R3			= 1 << 2,
	INPUT_PAD_START		= 1 << 3,
	INPUT_PAD_UP			= 1 << 4,
	INPUT_PAD_RIGHT		= 1 << 5,
	INPUT_PAD_DOWN		= 1 << 6,
	INPUT_PAD_LEFT		= 1 << 7,
	INPUT_PAD_L2			= 1 << 8,
	INPUT_PAD_R2			= 1 << 9,
	INPUT_PAD_L1			= 1 << 10,
	INPUT_PAD_R1			= 1 << 11,
	INPUT_PAD_TRIANGLE	= 1 << 12,
	INPUT_PAD_CIRCLE		= 1 << 13,
	INPUT_PAD_CROSS		= 1 << 14,
	INPUT_PAD_SQUARE		= 1 << 15,
} I_Button;

void I_Init(void);
int I_Poll(I_InputState *state, I_Port port);

#endif
