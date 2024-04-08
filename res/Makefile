
SRC_IMAGES := $(wildcard ./*.xcf)
OUT_IMAGES := $(SRC_IMAGES:./%.xcf=../bin/%.png)

REMOVE = rm -f

.phony: all images clean clean_images 

.SUFFIXES: .xcf .png 

all: images

images: $(OUT_IMAGES)
	cp ./*.png ../bin/

../bin/%.png: ./%.xcf
	convert $< -background none -flatten $@


clean_images:
	$(REMOVE) $(OUT_IMAGES)

clean: clean_images 

