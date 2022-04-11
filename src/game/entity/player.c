#include "player.h"
#include "../../input/input.h"

void d_E_FreeCam(G_State *state, G_Entity *self)
{

	SVECTOR	trot;			// Rotation value for matrix calculations
	trot.vx = state->camera.rotation.vx;
	trot.vy = state->camera.rotation.vy;
	trot.vz = state->camera.rotation.vz;

	I_InputState input;
	I_Poll(&input, I_PORT_DEFAULT);

			// Look controls
	if( (input.buttons&I_PAD_UP) ) {
		
		// Look up
		state->camera.rotation.vx -= 8;
		
	} else if( (input.buttons&I_PAD_DOWN) ) {
		
		// Look down
		state->camera.rotation.vx += 8;
		
	}
	
	if( (input.buttons&I_PAD_LEFT) ) {
		
		// Look left
		state->camera.rotation.vy += 8;
		
	} else if( (input.buttons&I_PAD_RIGHT) ) {
		
		// Look right
		state->camera.rotation.vy -= 8;
		
	}
	
	// Movement controls
	if( (input.buttons&I_PAD_TRIANGLE) ) {
		
		// Move forward
		state->camera.position.vx -= (( isin( trot.vy )*icos( trot.vx ) )>>12)<<4;
		state->camera.position.vy += isin( trot.vx )<<4;
		state->camera.position.vz += (( icos( trot.vy )*icos( trot.vx ) )>>12)<<4;
		
	} else if( (input.buttons&I_PAD_CROSS) ) {
		
		// Move backward
		state->camera.position.vx += (( isin( trot.vy )*icos( trot.vx ) )>>12)<<4;
		state->camera.position.vy -= isin( trot.vx )<<4;
		state->camera.position.vz -= (( icos( trot.vy )*icos( trot.vx ) )>>12)<<4;
		
	}
	
	if( (input.buttons&I_PAD_SQUARE ) ) {
		
		// Slide left
		state->camera.position.vx -= icos( trot.vy )<<4;
		state->camera.position.vz -= isin( trot.vy )<<4;
		
	} else if( (input.buttons&I_PAD_CIRCLE ) ) {
		
		// Slide right
		state->camera.position.vx += icos( trot.vy )<<4;
		state->camera.position.vz += isin( trot.vy )<<4;
		
	}
	
	if( (input.buttons&I_PAD_R1) ) {
		
		// Slide up
		state->camera.position.vx -= (( isin( trot.vy )*isin( trot.vx ) )>>12)<<4;
		state->camera.position.vy -= icos( trot.vx )<<4;
		state->camera.position.vz += (( icos( trot.vy )*isin( trot.vx ) )>>12)<<4;
		
	}
	
	if( (input.buttons&I_PAD_R2) ) {
		
		// Slide down
		state->camera.position.vx += (( isin( trot.vy )*isin( trot.vx ) )>>12)<<2;
		state->camera.position.vy += icos( trot.vx )<<2;
		state->camera.position.vz -= (( icos( trot.vy )*isin( trot.vx ) )>>12)<<2;
		
	}
	
	if( (input.buttons&I_PAD_L1) ) {
		
		state->camera.FOV+=16;
		R_SetFov(state->camera.FOV);
	}
	
	if( (input.buttons&I_PAD_L2) ) {
		
		state->camera.FOV-=16;
		R_SetFov(state->camera.FOV);
		
	}
}