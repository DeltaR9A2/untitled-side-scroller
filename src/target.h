#ifndef target_h
#define target_h

typedef struct action_t action_t;
typedef struct target_t target_t;

#include "rect.h"

void load_targets(char *fn);
void activate_closest_target_at(rect_t *rect);

#endif
