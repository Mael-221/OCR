# Simple SDL mini Makefile

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3 -lm -ldl
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image -lm -ldl

all: main

main: main.o Tools/tools.o segmentation/histogram.o
main.o : Tools/tools.h segmentation/histogram.h




clean:
	${RM} *.o
	${RM} *.d
	${RM} display
	${RM} grayscale

# END