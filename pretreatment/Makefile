# Simple SDL mini Makefile

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3 -lm -ldl
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image -lm -ldl

all: main

main: main.o pixel_operations.o grayscale.o binari.o contrast.o noise.o rotation.o
main.o : pixel_operations.h grayscale.h binari.h contrast.h noise.h rotation.o

clean:
	${RM} *.o
	${RM} *.d
	${RM} display
	${RM} grayscale

# END
