#ifndef controller_h
#define controller_h

#include <stdbool.h>

#include <SDL.h>

extern uint32_t KEYMAP_L;
extern uint32_t KEYMAP_R;
extern uint32_t KEYMAP_U;
extern uint32_t KEYMAP_D;

extern uint32_t KEYMAP_A;
extern uint32_t KEYMAP_B;
extern uint32_t KEYMAP_X;
extern uint32_t KEYMAP_Y;

extern uint32_t KEYMAP_LB;
extern uint32_t KEYMAP_RB;
extern uint32_t KEYMAP_LT;
extern uint32_t KEYMAP_RT;
	
extern uint32_t KEYMAP_LS;
extern uint32_t KEYMAP_RS;
extern uint32_t KEYMAP_BACK;
extern uint32_t KEYMAP_START;

extern const uint32_t BTN_L;
extern const uint32_t BTN_R;
extern const uint32_t BTN_U;
extern const uint32_t BTN_D;
	
extern const uint32_t BTN_A;
extern const uint32_t BTN_B;
extern const uint32_t BTN_X;
extern const uint32_t BTN_Y;
	
extern const uint32_t BTN_LB;
extern const uint32_t BTN_RB;
extern const uint32_t BTN_LT;
extern const uint32_t BTN_RT;
	
extern const uint32_t BTN_LS;
extern const uint32_t BTN_RS;
extern const uint32_t BTN_BACK;
extern const uint32_t BTN_START;

extern const uint32_t BTN_NONE;

typedef struct controller_t controller_t;

void controller_init(void);

void controller_reset(void);
void controller_poll_events(void);

bool controller_pressed(uint32_t buttons);
bool controller_released(uint32_t buttons);

bool controller_just_pressed(uint32_t buttons);
bool controller_just_released(uint32_t buttons);

#endif
