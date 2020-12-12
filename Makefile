# Simple SDL mini Makefile

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl gtk+-3.0` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3 -lm -ldl 
LDFLAGS=
LDLIBS= `pkg-config --libs sdl gtk+-3.0` -lSDL_image -lm -ldl

all: gui

gui: gui.o Tools/tools.o segmentation/histogram.o pretreatment/binari.o pretreatment/grayscale.o pretreatment/contrast.o pretreatment/noise.o  pretreatment/rotation.o NeuralNetwork/Ntools.o NeuralNetwork/neural_network.o
gui.o : Tools/tools.h segmentation/histogram.h pretreatment/binari.h pretreatment/grayscale.h pretreatment/contrast.h pretreatment/noise.h segmentation2/image_conversion.h pretreatment/rotation.h NeuralNetwork/Ntools.h NeuralNetwork/neural_network.h



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
	${RM} gui

# END
