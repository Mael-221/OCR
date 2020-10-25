# Simple SDL mini Makefile

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3 -lm -ldl
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image -lm -ldl

all: main

main: main.o Tools/tools.o segmentation/histogram.o pretreatment/binari.o pretreatment/grayscale.o pretreatment/contrast.o pretreatment/noise.o NeuralNetwork/NN_xor.o segmentation2/segmentation.o 
main.o : Tools/tools.h segmentation/histogram.h pretreatment/binari.h pretreatment/grayscale.h pretreatment/contrast.h pretreatment/noise.h NeuralNetwork/NN_xor.h segmentation2/segmentation.h 



clean:
	${RM} *.o
	${RM} *.d
	${RM} segmentation/*.o
	${RM} segmentation/*.d
	${RM} Tools/*.o
	${RM} Tools/*.d
	${RM} pretreatment/*.o
	${RM} pretreatment/*.d
	${RM} NeuralNetwork/*.o
	${RM} NeuralNetwork/*.d
	${RM} /segmentation2/*.d
	${RM} /segmentation2/*.o
	${RM} main

# END