#ifndef TOOLS_H
#define TOOLS_H

# include <stdlib.h>
# include <stdio.h>
# include <err.h>
#include <math.h>
#include <time.h>
#include<string.h>
#include "neural_network.h"

double sigmoid(double x);

double derivative_sigmoid(double x);

double Random();

double *File_to_list(char *filename);

double *goalArray(char letter);

double **goalMatrix();

double **lettersMatrix(char count);

void save(struct Neural_Network *net);

struct Neural_Network* load ();

int Get_Pos(struct Neural_Network *net);

void SquaredError(struct Neural_Network *net);

int Goal_Pos(double *goal);

char Get_Char(int val);




#endif
