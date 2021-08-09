#ifndef body_h
#define body_h

#include "rect.h"

extern const uint32_t BLOCKED_L;
extern const uint32_t BLOCKED_R;
extern const uint32_t BLOCKED_U;
extern const uint32_t BLOCKED_D;
extern const uint32_t BLOCKED_MASK;

extern const uint32_t PLAT_DROP;

typedef struct body_t body_t;
struct body_t{
	rect_t *rect;
	double vx, vy;
	uint32_t flags;
};

body_t *body_create(void);
void body_delete(body_t *body);

#endif
