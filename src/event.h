#ifndef event_h
#define event_h

typedef struct event_t event_t;

#include <stdlib.h>
#include <stdint.h>

struct event_t{
  int lua_ref;
  char config[64];
};

event_t *event_create(void);
void event_delete(event_t *event);

#endif
