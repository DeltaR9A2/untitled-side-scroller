#ifndef anim_h
#define anim_h

typedef struct anim_t anim_t;

#include "sdl_helpers.h"

#include "fset.h"

/**
This struct holds the definition of a single animation.
Intended to be created/altered/destroyed using the functions in anim.h
*/
struct anim_t{
	int fps; /**< The rate of the animation, in frames per second. */
	int len; /**< The number of frames in this animation. */
	SDL_Surface **frames; /**< An array of pointers to the frame surfaces. */
};

/** Create a new animation and return a pointer to it. */
anim_t *anim_create(void);


void anim_init(anim_t *anim, fset_t *fset, int start, int len, int fps);

/**
Delete an animation created by anim_create.
Does not delete the frame surfaces; those are freed
when the frameset is deleted. 
*/
void anim_delete(anim_t *anim);

/** Draw the current frame of an animation to a target surface & rect. */
void anim_draw(anim_t *anim, int step, SDL_Surface *target, SDL_Rect *dest);

#endif
