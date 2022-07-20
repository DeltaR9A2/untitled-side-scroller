
# Standard Build ##############
# export OS := linux
export CC := gcc
export PC := pkg-config
export PACKAGES := sdl2
export TARGET := game
export CFLAGS := -std=c11 -O2 `$(PC) --cflags $(PACKAGES)` -I./src
export LFLAGS := -Wl,-rpath='$$ORIGIN/lib' `$(PC) --libs $(PACKAGES)` -lm
export REMOVE  := rm -rf
###############################

# Debug Flags #####
debug : export CFLAGS := $(CFLAGS) -g -Wall -Werror -D DEBUG -O0
debug : export LFLAGS := $(LFLAGS)
###################

.PHONY: all src res clean_src clean_res clean run $(TARGET) debug

all: $(TARGET)

res:
	$(MAKE) -C res

src:
	$(MAKE) -C src

$(TARGET): src res
	$(CC) ./obj/*.o $(LFLAGS) -o ./bin/$@

run: $(TARGET)
	@(cd bin/ && exec ./$(TARGET))

clean_src:
	$(MAKE) -C src clean
	$(REMOVE) ./bin/$(TARGET)

clean_res:
	$(MAKE) -C res clean

clean: clean_src clean_res
	$(REMOVE) ./bin/*
	$(REMOVE) ./obj/*

debug: all

mingw: all

