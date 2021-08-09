#include <stdlib.h>
#include <stdio.h>

#include "controller.h"

uint32_t KEYMAP_L = SDL_SCANCODE_LEFT;
uint32_t KEYMAP_R = SDL_SCANCODE_RIGHT;
uint32_t KEYMAP_U = SDL_SCANCODE_UP;
uint32_t KEYMAP_D = SDL_SCANCODE_DOWN;

uint32_t KEYMAP_A = SDL_SCANCODE_Z;
uint32_t KEYMAP_B = SDL_SCANCODE_X;
uint32_t KEYMAP_X = SDL_SCANCODE_C;
uint32_t KEYMAP_Y = SDL_SCANCODE_V;

uint32_t KEYMAP_LB =   SDL_SCANCODE_F1;
uint32_t KEYMAP_RB =   SDL_SCANCODE_F2;
uint32_t KEYMAP_LT =   SDL_SCANCODE_1;
uint32_t KEYMAP_RT =   SDL_SCANCODE_2;
	
uint32_t KEYMAP_LS =    SDL_SCANCODE_F3;
uint32_t KEYMAP_RS =    SDL_SCANCODE_F4;
uint32_t KEYMAP_BACK =  SDL_SCANCODE_BACKSPACE;
uint32_t KEYMAP_START = SDL_SCANCODE_RETURN;

const uint32_t BTN_L =	  0x00000001;
const uint32_t BTN_R =	  0x00000002;
const uint32_t BTN_U =	  0x00000004;
const uint32_t BTN_D =	  0x00000008;
const uint32_t BTN_DIR_ANY = 0x0000000F;

const uint32_t BTN_A =	  0x00000010;
const uint32_t BTN_B =	  0x00000020;
const uint32_t BTN_X =	  0x00000040;
const uint32_t BTN_Y =	  0x00000080;
	
const uint32_t BTN_LB =	  0x00000100;
const uint32_t BTN_RB =	  0x00000200;
const uint32_t BTN_LT =	  0x00000400;
const uint32_t BTN_RT =	  0x00000800;
	
const uint32_t BTN_LS =	  0x00001000;
const uint32_t BTN_RS =	  0x00002000;
const uint32_t BTN_BACK = 0x00004000;
const uint32_t BTN_START = 0x00008000;

const uint32_t BTN_NONE = 0x00000000;

controller_t *controller_create(void){
	controller_t *controller = malloc(sizeof(controller_t));

	controller->pressed = BTN_NONE;
	controller->previous = BTN_NONE;
	
	#ifdef DEBUG
	printf("Joystick Count: %i\n", SDL_NumJoysticks());
	printf("Joystick Event State: %i\n", SDL_JoystickEventState(SDL_ENABLE));
	#endif
	
	SDL_JoystickOpen(0);
	
	return controller;
}

void controller_delete(controller_t *controller){
	free(controller);
}

bool controller_pressed(controller_t *c, uint32_t buttons){
	return ((c->pressed & buttons) == buttons);
}

bool controller_released(controller_t *c, uint32_t buttons){
	return !((c->pressed & buttons) == buttons);
}

bool controller_just_pressed(controller_t *c, uint32_t buttons){
	return ((c->pressed & buttons) == buttons) && !((c->previous & buttons) == buttons);
}

bool controller_just_released(controller_t *c, uint32_t buttons){
	return !((c->pressed & buttons) == buttons) && ((c->previous & buttons) == buttons);
}

void controller_reset(controller_t *c){
	c->pressed = BTN_NONE;
	c->previous = BTN_NONE;
}

void controller_poll_events(controller_t *c){
	SDL_Event e;

	c->previous = c->pressed;
	while(SDL_PollEvent(&e)){
		if(e.type == SDL_KEYDOWN){
			uint32_t key = e.key.keysym.scancode;
			if(key == KEYMAP_U){
				c->pressed |= BTN_U;
			}else if(key == KEYMAP_D){
				c->pressed |= BTN_D;
			}else if(key == KEYMAP_L){
				c->pressed |= BTN_L;
			}else if(key == KEYMAP_R){
				c->pressed |= BTN_R;
			}else if(key == KEYMAP_A){
				c->pressed |= BTN_A;
			}else if(key == KEYMAP_B){
				c->pressed |= BTN_B;
			}else if(key == KEYMAP_X){
				c->pressed |= BTN_X;
			}else if(key == KEYMAP_Y){
				c->pressed |= BTN_Y;
			}else if(key == KEYMAP_LB){
				c->pressed |= BTN_LB;
			}else if(key == KEYMAP_RB){
				c->pressed |= BTN_RB;
			}else if(key == KEYMAP_LT){
				c->pressed |= BTN_LT;
			}else if(key == KEYMAP_RT){
				c->pressed |= BTN_RT;
			}else if(key == KEYMAP_START){
				c->pressed |= BTN_START;
			}else if(key == KEYMAP_BACK){
				c->pressed |= BTN_BACK;
			}
		}else if(e.type == SDL_KEYUP){
			uint32_t key = e.key.keysym.scancode;
			if(key == KEYMAP_U){
				c->pressed &= ~BTN_U;
			}else if(key == KEYMAP_D){
				c->pressed &= ~BTN_D;
			}else if(key == KEYMAP_L){
				c->pressed &= ~BTN_L;
			}else if(key == KEYMAP_R){
				c->pressed &= ~BTN_R;
			}else if(key == KEYMAP_A){
				c->pressed &= ~BTN_A;
			}else if(key == KEYMAP_B){
				c->pressed &= ~BTN_B;
			}else if(key == KEYMAP_X){
				c->pressed &= ~BTN_X;
			}else if(key == KEYMAP_Y){
				c->pressed &= ~BTN_Y;
			}else if(key == KEYMAP_LB){
				c->pressed &= ~BTN_LB;
			}else if(key == KEYMAP_RB){
				c->pressed &= ~BTN_RB;
			}else if(key == KEYMAP_LT){
				c->pressed &= ~BTN_LT;
			}else if(key == KEYMAP_RT){
				c->pressed &= ~BTN_RT;
			}else if(key == KEYMAP_START){
				c->pressed &= ~BTN_START;
			}else if(key == KEYMAP_BACK){
				c->pressed &= ~BTN_BACK;
			}
		}else if(e.type == SDL_JOYHATMOTION){
			c->pressed &= ~BTN_DIR_ANY;
			if(e.jhat.value == SDL_HAT_UP){
				c->pressed |= BTN_U;
			}else if(e.jhat.value == SDL_HAT_LEFT){
				c->pressed |= BTN_L;
			}else if(e.jhat.value == SDL_HAT_RIGHT){
				c->pressed |= BTN_R;
			}else if(e.jhat.value == SDL_HAT_DOWN){
				c->pressed |= BTN_D;
			}else if(e.jhat.value == SDL_HAT_LEFTUP){
				c->pressed |= BTN_L | BTN_U;
			}else if(e.jhat.value == SDL_HAT_RIGHTUP){
				c->pressed |= BTN_R | BTN_U;
			}else if(e.jhat.value == SDL_HAT_LEFTDOWN){
				c->pressed |= BTN_L | BTN_D;
			}else if(e.jhat.value == SDL_HAT_RIGHTDOWN){
				c->pressed |= BTN_R | BTN_D;
			}
		}else if(e.type == SDL_JOYBUTTONDOWN){
			#ifdef DEBUG
			printf("JOYBUTTON %i \n", e.jbutton.button);
			#endif
			
			if(e.jbutton.button == 0){
				c->pressed |= BTN_A;
			}else if(e.jbutton.button == 1){
				c->pressed |= BTN_B;
			}else if(e.jbutton.button == 2){
				c->pressed |= BTN_X;
			}else if(e.jbutton.button == 3){
				c->pressed |= BTN_Y;
			}else if(e.jbutton.button == 4){
				c->pressed |= BTN_LB;
			}else if(e.jbutton.button == 5){
				c->pressed |= BTN_RB;
			}else if(e.jbutton.button == 6){
				c->pressed |= BTN_BACK;
			}else if(e.jbutton.button == 7){
				c->pressed |= BTN_START;
			}
		}else if(e.type == SDL_JOYBUTTONUP){
			if(e.jbutton.button == 0){
				c->pressed &= ~BTN_A;
			}else if(e.jbutton.button == 1){
				c->pressed &= ~BTN_B;
			}else if(e.jbutton.button == 2){
				c->pressed &= ~BTN_X;
			}else if(e.jbutton.button == 3){
				c->pressed &= ~BTN_Y;
			}else if(e.jbutton.button == 4){
				c->pressed &= ~BTN_LB;
			}else if(e.jbutton.button == 5){
				c->pressed &= ~BTN_RB;
			}else if(e.jbutton.button == 6){
				c->pressed &= ~BTN_BACK;
			}else if(e.jbutton.button == 7){
				c->pressed &= ~BTN_START;
			}
		}
	}
}
