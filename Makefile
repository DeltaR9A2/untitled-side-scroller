
# Linux Build #################
export OS := linux
export CC := gcc
export PC := pkg-config

# Windows Build ###############
# export OS := windows
# export CC := x86_64-w64-mingw32-gcc
# export PC := x86_64-w64-mingw32-pkg-config

###############################
export PACKAGES := sdl2
export TARGET := game
export CFLAGS := -std=c11 -O2 `$(PC) --cflags $(PACKAGES)` -I./src
export LFLAGS := -Wl,-rpath='$$ORIGIN/lib' `$(PC) --libs $(PACKAGES)` -lm
export REMOVE  := rm -rf
###############################

SOURCES := $(wildcard ./src/*.c)
OBJECTS := $(SOURCES:./src/%.c=./obj/%.o)

# Debug Flags #####
debug : export CFLAGS := $(CFLAGS) -g -Wall -Werror -D DEBUG -O0
debug : export LFLAGS := $(LFLAGS)
###################

.PHONY: all src res clean_src clean_res clean run $(TARGET) debug

all: $(TARGET)

res:
	cp ./res/* ./bin/

src: $(OBJECTS)

./obj/%.o: ./src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): src res
	$(CC) ./obj/*.o $(LFLAGS) -o ./bin/$@

run: $(TARGET)
	@(cd bin/ && exec ./$(TARGET))

clean_src:
	$(MAKE) -C src clean
	$(REMOVE) ./bin/$(TARGET)

clean: clean_src clean_res
	$(REMOVE) ./bin/*
	$(REMOVE) ./obj/*

debug: all

mingw: all

