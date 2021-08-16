
SOURCES := $(wildcard ./*.c)
OBJECTS := $(SOURCES:./%.c=../obj/%.o)

REMOVE := rm -f

.phony: all obj clean

all:
	$(MAKE) obj

obj: $(OBJECTS)

../obj/%.o: ./%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(REMOVE) $(OBJECTS)

