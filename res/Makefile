
SRC_IMAGES := $(wildcard ./*.xcf)
OUT_IMAGES := $(SRC_IMAGES:./%.xcf=../bin/%.png)

SRC_SOUNDS := $(wildcard ./*.wav)
OUT_SOUNDS := $(SRC_SOUNDS:./%.wav=../bin/%.ogg)

SRC_MUSIC := $(wildcard ./*.mp3)
OUT_MUSIC := $(SRC_MUSIC:./%.mp3=../bin/%.ogg)

REMOVE = rm -f

.phony: all images sounds music clean clean_images clean_sounds clean_music

.SUFFIXES: .xcf .png .wav .ogg .mp3

all: images sounds music

images: $(OUT_IMAGES)

../bin/%.png: ./%.xcf
	convert $< -background none -flatten $@

sounds: $(OUT_SOUNDS)

../bin/%.ogg: ./%.wav
	ffmpeg -v 0 -i $< -vn -codec:a libvorbis $@

music: $(OUT_MUSIC)

../bin/%.ogg: ./%.mp3
	ffmpeg -v 0 -i $< -vn -codec:a libvorbis $@

clean_images:
	$(REMOVE) $(OUT_IMAGES)

clean_sounds:
	$(REMOVE) $(OUT_SOUNDS)

clean_music:
	$(REMOVE) $(OUT_MUSIC)

clean: clean_images clean_sounds clean_music

