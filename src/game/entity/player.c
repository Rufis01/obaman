#include "player.h"
#include "../../input/input.h"

bool E_PacmanMovement(G_State *state, G_Entity *self)
{
	(void)state;
	if(self->type != ENTITY_PLAYER)
		return false;
	return true;
}

bool d_E_FreeCam(G_State *state, G_Entity *self, struct _E_BehaviourHeader* header)
{
	(void)self;
	(void)header;
	SVECTOR	trot;			// Rotation value for matrix calculations
	trot.vx = state->camera.rotation.vx;
	trot.vy = state->camera.rotation.vy;
	trot.vz = state->camera.rotation.vz;

	I_InputState input = {0};
	I_Poll(&input, INPUT_PORT_DEFAULT);

			// Look controls
	if( (input.buttons&INPUT_PAD_UP) ) {
		printf("Look up!\n");
		// Look up
		state->camera.rotation.vx -= 8;
		
	} else if( (input.buttons&INPUT_PAD_DOWN) ) {
		
		// Look down
		state->camera.rotation.vx += 8;
		
	}
	
	if( (input.buttons&INPUT_PAD_LEFT) ) {
		
		// Look left
		state->camera.rotation.vy += 8;
		
	} else if( (input.buttons&INPUT_PAD_RIGHT) ) {
		
		// Look right
		state->camera.rotation.vy -= 8;
		
	}
	
	// Movement controls
	if( (input.buttons&INPUT_PAD_TRIANGLE) ) {
		
		// Move forward
		state->camera.position.vx -= (( isin( trot.vy )*icos( trot.vx ) )>>12)<<4;
		state->camera.position.vy += isin( trot.vx )<<4;
		state->camera.position.vz += (( icos( trot.vy )*icos( trot.vx ) )>>12)<<4;
		
	} else if( (input.buttons&INPUT_PAD_CROSS) ) {
		
		// Move backward
		state->camera.position.vx += (( isin( trot.vy )*icos( trot.vx ) )>>12)<<4;
		state->camera.position.vy -= isin( trot.vx )<<4;
		state->camera.position.vz -= (( icos( trot.vy )*icos( trot.vx ) )>>12)<<4;
		
	}
	
	if( (input.buttons&INPUT_PAD_SQUARE ) ) {
		
		// Slide left
		state->camera.position.vx -= icos( trot.vy )<<4;
		state->camera.position.vz -= isin( trot.vy )<<4;
		
	} else if( (input.buttons&INPUT_PAD_CIRCLE ) ) {
		
		// Slide right
		state->camera.position.vx += icos( trot.vy )<<4;
		state->camera.position.vz += isin( trot.vy )<<4;
		
	}
	
	if( (input.buttons&INPUT_PAD_R1) ) {
		
		// Slide up
		state->camera.position.vx -= (( isin( trot.vy )*isin( trot.vx ) )>>12)<<4;
		state->camera.position.vy -= icos( trot.vx )<<4;
		state->camera.position.vz += (( icos( trot.vy )*isin( trot.vx ) )>>12)<<4;
		
	}
	
	if( (input.buttons&INPUT_PAD_R2) ) {
		
		// Slide down
		state->camera.position.vx += (( isin( trot.vy )*isin( trot.vx ) )>>12)<<2;
		state->camera.position.vy += icos( trot.vx )<<2;
		state->camera.position.vz -= (( icos( trot.vy )*isin( trot.vx ) )>>12)<<2;
		
	}
	
	if( (input.buttons&INPUT_PAD_L1) ) {
		
		state->camera.FOV+=16;
#pragma GCC diagnostic ignored "-Wimplicit-function-declaration"
		R_SetFov(state->camera.FOV);
	}
	
	if( (input.buttons&INPUT_PAD_L2) ) {
		
		state->camera.FOV-=16;
#pragma GCC diagnostic ignored "-Wimplicit-function-declaration"
		R_SetFov(state->camera.FOV);
		
	}

	return true;
}

bool d_E_ChainTest(G_State *state, G_Entity *self, struct _E_BehaviourHeader* header)
{
	(void)self;
	(void)header;

	I_InputState input = {0};
	I_Poll(&input, INPUT_PORT_DEFAULT);

	if(input.buttons & INPUT_PAD_START)
		return false;
	if(input.buttons & INPUT_PAD_SELECT)
		printf("Congrats, it's working :D\n");
		
	return true;
}