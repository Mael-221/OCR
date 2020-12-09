#ifndef RECONSTRUCT_CHAR_H_
#define RECONSTRUCT_CHAR_H_

#include <stdlib.h>
#include <stdio.h>

int Net_Pos(struct Neural_Network *net);
char Net_Char(int val);
char ReconstructChar(struct Neural_Network *net, double* letter);

# endif
