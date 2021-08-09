
SOURCES := $(wildcard ./*.c)
OBJECTS := $(SOURCES:./%.c=../obj/%.o)

LUA_SOURCES := $(wildcard ./lua/*.lua)
LUA_OUTPUTS := $(LUA_SOURCES:./lua/%.lua=../bin/%.lua)

REMOVE := rm -f

.phony: all obj clean

all:
	$(MAKE) -C gen
	$(MAKE) obj
	$(MAKE) lua

obj: $(OBJECTS)

../obj/%.o: ./%.c
	$(CC) $(CFLAGS) -c $< -o $@

lua: $(LUA_OUTPUTS)

../bin/%.lua: ./lua/%.lua
	cp $< $@

clean:
	$(MAKE) -C gen clean
	$(REMOVE) $(OBJECTS)
	$(REMOVE) $(LUA_OUTPUTS)

