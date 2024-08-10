CC=cc
CFLAGS=-O -I include/
VPATH= src include

all: raytraycer

raytraycer: main.c utility.o vec3.o world.o camera.o rendering.o
	$(CC) $(CFLAGS) $^ -lm -o $@
	mv *.o $@ build/

camera.o: camera.h utility.h vec3.h
world.o: world.h utility.h vec3.h
vec3.o: vec3.h utility.h
utility.o: utility.h
rendering.o: rendering.h utility.h vec3.h camera.h

.PHONY: clean
clean: 
	-rm -f build/* *.o

