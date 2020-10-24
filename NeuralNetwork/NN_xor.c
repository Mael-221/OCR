#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "NN_xor.h"
#include <stddef.h>
#include <time.h>


//Activation function
double sigmoid(double x) 
{
    return 1.0/(1.0+exp(-x));
}
double derivative_sigmoid(double x) 
{
    return x*(1.0 - x);
}
double Grandom() 
{
    return ((double)rand()) / ((double)RAND_MAX);
}


struct Neural_Network
{
    //Number of nodes per layer
    int nbInput;
    int nbHidden;
    int nbOutput;

    //Arrays of the activation values of each node
    double *hiddenLayer;
    double output;
    //Bias
    double *hiddenBias;
    double  outputBias;
    //Weights
    double *hiddenWeights;
    double *outputWeights;
    //Inputs and targeted outputs
    double *input_values;
    double *target;
    

    //Delta values for backpropapagation;
    double *deltaOutput;
    double *deltaHidden;
    double *dBiasH;
    double dBiasO;
};

struct Neural_Network Initialize_network()
{
    struct Neural_Network net;
    net.nbInput=2;
    net.nbHidden=2;
    net.nbOutput=1;
    net.hiddenLayer=malloc(sizeof(double)*(net).nbHidden);
    net.hiddenBias=malloc(sizeof(double)*(net).nbHidden);
    net.outputBias=Grandom();
    net.dBiasH=malloc(sizeof(double)*(net).nbHidden);
    net.dBiasO=0.0;


    net.hiddenWeights=malloc(sizeof(double)*(net).nbInput*net.nbHidden);
    net.outputWeights=malloc(sizeof(double)*(net).nbHidden);
    net.input_values=malloc(sizeof(double)*8);
    net.target=malloc(sizeof(double)*4);
    net.deltaHidden=malloc(sizeof(double)*(net).nbInput);
    net.deltaOutput=malloc(sizeof(double)*(net).nbHidden);




    return(net);

}


//initialize the different values

void Initialize_values(struct Neural_Network *net) 

{
    (*net).input_values[0]=1;
    (*net).input_values[1]=1;
    (*net).input_values[2]=0;
    (*net).input_values[3]=0;
    (*net).input_values[4]=1;
    (*net).input_values[5]=0;
    (*net).input_values[6]=0;
    (*net).input_values[7]=1;

    (*net).target[0]=0;
    (*net).target[1]=0;
    (*net).target[2]=1;
    (*net).target[3]=1;



    for(int i=0;i< (*net).nbHidden;i++)
    {
        (*net).hiddenBias[i]=Grandom();
        (*net).dBiasH[i]=0.0;

    }

    
     (*net).outputBias=Grandom();
     

    

    for(int i=0;i< (*net).nbInput;i++)
    {
        for(int j=0;j< (*net).nbHidden;j++)
        {
            (*net).hiddenWeights[i+j*(*net).nbHidden]=Grandom();

        }

    }
      for(int i=0;i< (*net).nbHidden;i++)
    {
        
        (*net).outputWeights[i]=Grandom();

        

    }


}

void print_weights_biases(struct Neural_Network *net)
{
        printf("\n");

    printf("HBias= ");
    for(int j=0; j<(*net).nbHidden;j++)
    {
        printf("%f ",(*net).hiddenBias[j]);
    }
    printf("\n");

    printf("IHWEIGHTS= ");
    for(int j=0; j<(*net).nbHidden*(*net).nbInput;j++)
    {
        printf("%f ",(*net).hiddenWeights[j]);
    }
    printf("\n");

    printf("OHWEIGHTS= ");
    for(int j=0; j<(*net).nbHidden*(*net).nbOutput;j++)
    {
        printf("%f ",(*net).outputWeights[j]);
    }
    printf("\n");



    printf("OBias= ");
    printf("%f ",(*net).outputBias);
    printf("\n");




}


void ForwardPass(struct Neural_Network *net,int pos,int epoch)
{

    for(int i=0;i<(*net).nbHidden;i++)
    {
        double Sum_h=0;

        for (int j=0;j<(*net).nbInput;j++)
        {
            Sum_h+=((*net).hiddenWeights[i+j*(*net).nbHidden])*
                ((*net).input_values[j+pos]);
        }

        (*net).hiddenLayer[i]=sigmoid(Sum_h+(*net).hiddenBias[i]);

    }
    
    
    double Sum_o=0;
    for(int j=0;j<(*net).nbHidden;j++)
    {
        Sum_o+= (*net).outputWeights[j]*
                (*net).hiddenLayer[j];
    }
    (*net).output=sigmoid(Sum_o+(*net).outputBias);

    

    if(epoch%1000==0)
    {
        printf("Input1=%d / Input2=%d / Output=%f / GOAL =%d \n"
                ,(int)(*net).input_values[pos]
                ,(int)(*net).input_values[pos+1]
                ,(*net).output
                ,(int)(*net).target[pos/2]);
        



    }

}
/*
void MuteWeights(struct Neural_Network *net,int pos)
{
   
    double dError=((*net).output-(*net).target[pos/2])
        *derivative_sigmoid((*net).output);
    for(int i=0;i<(*net).nbHidden;i++)
    {
        (*net).deltaOutput[i]=dError*(*net).hiddenLayer[i];
    }
    for(int i=0;i<(*net).nbHidden;i++)
    {
        double Nerror=dError*(*net).outputWeights[i];
        for(int j=0;j<(*net).nbInput;j++)
        {
            (*net).deltaHidden[i+j*(*net).nbHidden]=Nerror*
                derivative_sigmoid((*net).hiddenLayer[i])
                *(*net).input_values[pos+j];
        }

    }

}

void MuteBiases(struct Neural_Network *net,int pos)
{
    double dError=((*net).output-(*net).target[pos/2])
        *derivative_sigmoid((*net).output);
    (*net).dBiasO+=dError;
    for(int i=0;i<(*net).nbHidden;i++)
    {
        (*net).dBiasH[i]+=dError*
            derivative_sigmoid((*net).hiddenLayer[i])
            *(*net).outputWeights[i];
    }

}
void ReinitializeDeltas(struct Neural_Network *net)
{
    (*net).dBiasO=0.0;
    (*net).output=0.0;
    for(int i=0;i<(*net).nbHidden;i++)
    {
        (*net).dBiasH[i]=0.0;
        (*net).deltaOutput[i]=0.0;
        (*net).hiddenLayer[i]=0.0;
        for(int j=0;j<(*net).nbInput;j++)
        {
            (*net).deltaHidden[i+j]=0.0;

        }


    }
}
void BackwardPass(struct Neural_Network *net,int pos)
{
    MuteWeights(net,pos);
    MuteBiases(net,pos);
    (*net).outputBias-=0.5*(*net).dBiasO;
    for(int i=0;i<(*net).nbHidden;i++)
    {
        (*net).outputWeights[i]-=(*net).deltaOutput[i];
        (*net).hiddenBias[i]-=0.5*(*net).dBiasH[i];
         for(int j=0;j<(*net).nbInput;j++)
        {
            (*net).hiddenWeights[i+j*(*net).nbHidden]-=
                (*net).deltaHidden[i+j*(*net).nbHidden];
        }


    }
    ReinitializeDeltas(net);

}*/



void BackwardPass(struct Neural_Network *net,int pos,double lr,int epoch)
{
   
    double dError=((*net).target[pos/2]-(*net).output);
    (*net).deltaOutput[0]=dError*derivative_sigmoid((*net).output);

    if(epoch%10000==0)
    {
        printf("dError+==%f \n"
                ,((*net).target[pos/2]-(*net).output));
    }

   
   for(int i =0;i<(*net).nbHidden;i++)
   {
         double dError=0.0f;
         dError+=(*net).deltaOutput[0]*
                (*net).outputWeights[i];
         
         (*net).deltaHidden[i]=dError*
             derivative_sigmoid((*net).hiddenLayer[i]);

   }
   
   (*net).outputBias+=(*net).deltaOutput[0]*lr;
   for(int j=0;j<(*net).nbHidden;j++)
   {
       (*net).outputWeights[j]+=(*net).hiddenLayer[j]*
                                (*net).deltaOutput[0]*lr;

    if(epoch%10000==0)
    {
        printf("Weight0H[%d]+==%f \n"
                ,j
                ,(*net).hiddenLayer[j]*(*net).deltaOutput[0]*lr);
    }

   }

   

     for(int i =0;i<(*net).nbHidden;i++)
   {
       (*net).hiddenBias[i]+=(*net).deltaHidden[i]*lr;
       for(int j=0;j<(*net).nbInput;j++)
       {
            (*net).hiddenWeights[i+j*(*net).nbHidden]+=
                (*net).input_values[pos+i]* (*net).deltaHidden[i]*lr;

        if(epoch%10000==0)
         {
            printf("WeightIH[%d]+==%f \n"
                ,i+j*(*net).nbHidden
                ,(*net).input_values[pos+j]* (*net).deltaHidden[i]*lr);
;
         }
    

       }

   }


}


void XOR()
{
    //int posi[4]={0,2,4,6};
    srand(time(NULL));
    int nbinputs=6;
    double lr=0.2;
    int nbepochs=100000;
    struct Neural_Network net1= Initialize_network();
    struct Neural_Network *net = &net1;
    Initialize_values(net);
    print_weights_biases(net);

    for(int i=0;i<nbepochs;i++)
    {
        /*int pos=posi[rand()%4];
        ForwardPass(net,pos,i);
        BackwardPass(net,pos,lr);*/

         /*if(nbepochs%1000==0)
         {
            print_weights_biases(net);
         }*/

        for(int j=0; j<=nbinputs;j+=2)
        {
            ForwardPass(net,j,i);
            BackwardPass(net,j,lr,i);


        }
        
    }

    print_weights_biases(net);



}







