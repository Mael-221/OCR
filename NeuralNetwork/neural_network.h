#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include <stdio.h>
#include <stdlib.h>

struct Neural_Network
{
    int nbInput;
    int nbHidden;
    int nbOutput;

    double Inputs[28*28];
    double Goals[52];
    double Hidden[20];
    double Outputs[52];


    double WeightsIH[28*28][20];
    double WeightsHO[20][52];
    double BiasH[20];
    double BiasO[52];


    double lr;
    double ErrorRate;
    double MaxErrorRate;
    double dHidden[20];
    double dOutput[52];


};

#include "tools.h"
struct Neural_Network* InitializeNetwork();
void OCR_Neural_Network(struct Neural_Network *net, double *inputs, double *goals);
void Train_Neural_Network(struct Neural_Network *net);
char list_to_char(struct Neural_Network *net,double *inputs);

#endif
