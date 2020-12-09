# include "neural_network.h"



struct Neural_Network* InitializeNetwork()
{
  struct Neural_Network *net ;
  net = malloc(sizeof(struct Neural_Network));
  net -> nbInput = 28*28;
  net -> nbHidden = 20;
  net -> nbOutput = 52;
  net->lr=0.1;
  net->ErrorRate=0.0;

  //Set Values
  for (int i = 0; i < net -> nbInput; i++)
  {
    for (int h = 0; h < net -> nbHidden; h++)
    {
      net -> WeightsIH[i][h] = Random();


    }
  }

  for (int h = 0; h < net -> nbHidden; h++)
  {
    for(int o = 0; o < net -> nbOutput; o++)
    {
      net -> WeightsHO[h][o] = Random();

    }
    net -> BiasH[h] = Random();
  }

  for (int o = 0; o < net -> nbOutput; o++)
  {
    net -> BiasO[o] = Random();
  }

  return net;
}

void ForwardPass(struct Neural_Network *net)
{

    for(int h=0;h<(*net).nbHidden;h++)
    {
        double Sum_h=0;

        for (int i=0;i<(*net).nbInput;i++)
        {
            Sum_h+=net->WeightsIH[i][h]*net->Inputs[i];
        }

        (*net).Hidden[h]=sigmoid(Sum_h+(*net).BiasH[h]);

    }


      //Calculate Output for Output neurons
  double Sumexp=0.0;
  for (int o = 0; o < net -> nbOutput; o++)
  {
    double Sum_O = 0.0;
    for (int h = 0; h < net -> nbHidden; h++)
    {
        Sum_O+=  net -> WeightsHO[h][o]* net -> Hidden[h];
    
    }
    
    net -> Outputs[o] = (Sum_O +  net-> BiasO[o]);
    Sumexp+=exp( net->Outputs[o]);
  }
  for (int o = 0; o < net -> nbOutput; o++)
  {
    net -> Outputs[o] = exp(net -> Outputs[o])/Sumexp; 
  }



}

void BackwardPass(struct Neural_Network *net)
{

    for(int o=0;o<net->nbOutput;o++)
    {
        double dError=net->Goals[o]-net->Outputs[o];
        (*net).dOutput[o]=dError;

    }

    for(int h=0;h<(*net).nbHidden;h++)
    {
        double Error=0.0f;
        for(int o=0;o<net->nbOutput;o++)
        {
                Error+= net->dOutput[o] * net->WeightsHO[h][o];
        }

         net->dHidden[h]=Error*derivative_sigmoid(net->Hidden[h]);

   }

   for(int o=0;o<net->nbOutput;o++)
   {
       (*net).BiasO[o]+=(*net).dOutput[o]*net->lr;
       for(int h=0;h<(*net).nbHidden;h++)
        {
            (*net).WeightsHO[h][o]+=net->Hidden[h]*
                                net->dOutput[o]*net->lr;

        }
    }


     for(int h =0;h<(*net).nbHidden;h++)
   {
       net->BiasH[h]+=net->dHidden[h]*net->lr;
       for(int i=0;i<net->nbInput;i++)
       {
            net->WeightsIH[i][h]+=
                (*net).Inputs[i]* net->dHidden[h]*net->lr;

       }

   }


}
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KWHT  "\x1B[37m"

void PrintState(struct Neural_Network *net)
{
  //Squared error function
  SquaredError(net);
 for(int k=0; k<net->nbHidden;k++)
     printf("%f ",net->WeightsIH[0][k]);
  printf("\n Hidden=[ ");
  for(int k=0; k<net->nbHidden;k++)
  {
      printf("%f, ",net->Hidden[k]);
  }
  printf("]\n");

  int output = (Get_Pos(net));

  //Retrive the chars : wanted & found
  int goalpos=Goal_Pos(net -> Goals);
  char goalChar = Get_Char(goalpos);
  char recognizedChar = Get_Char(output);



  //Print the progress
  if(output == goalpos )
    printf("Position Found = %d Expected %d %sOK \n",
                    output,goalpos,KGRN);
  else
    printf("Position Found = %d Expected %d %sKO \n",
                    output, goalpos ,KRED);

  printf("%s",KWHT);

  printf("Char entered: %c | Char recoginized: %c | ErrorRate: %f\n",
                                                    goalChar,
                                                    recognizedChar,
                                                    net -> ErrorRate);
 /* for(int k=0; k<28*28;k++)
  {
      if(k%28==0)
      {
          printf("\n%d ",(int) net->Inputs[k]);
      }
      else
          printf("%d ",(int)net->Inputs[k]);

  }*/

}

void OCR_Neural_Network(struct Neural_Network *net, double *inputs, double *goals)
{
      //Initialise Goals & InputValues for this char
  for (int g = 0; g < net->nbOutput; g++)
  {
    net -> Goals[g] = goals[g];
  }
  for(int i = 0; i < net -> nbInput; i++)
  {
    net -> Inputs[i] = inputs[i];
  }

  ForwardPass(net);
  BackwardPass(net);

}

void Train_Neural_Network(struct Neural_Network *net)
{
    int epoch=10000;
    double **goals=goalMatrix();
    
    double **inputs1=lettersMatrix('1');
   double **inputs2=lettersMatrix('2');
    double **inputs3=lettersMatrix('3');
    double **inputs4=lettersMatrix('4');
    double **inputs;

    for(int i=0;i<=epoch;i++)
    {
        for(int k=0;k<4;k++)
        {
            switch (k)
            {
                case 0:
                    inputs=inputs1;
                    break;
                 case 1:
                    inputs=inputs2;
                    break;
                 case 2:
                    inputs=inputs3;
                    break;
                 case 3:
                    inputs=inputs4;
                    break;
                default:
                        inputs=inputs4;
                        break;


     }

            for(int j=0;j<52;j++)
            {
                
                OCR_Neural_Network(net,inputs[j],goals[j]);

                if(i%1000==0 || i==1)
                {
                    
                    PrintState(net);

                }

            }
                    
        }

    }
    save(net);

    
}

char list_to_char(struct Neural_Network *net,double *inputs)
{
    for(int i = 0; i < net -> nbInput; i++)
  {
    net -> Inputs[i] = inputs[i];
  }
    for(int k=0; k<28*28;k++)
     {
         if(k%28==0)
        {
            printf("\n%d ",(int) net->Inputs[k]);
        }
         else
             printf("%d ",(int)net->Inputs[k]);
     }

    ForwardPass(net);
    printf("\n Output=[ ");
    for(int k=0; k<52;k++)
    {
        printf("%f, ",net->Outputs[k]);
    }
    printf("]\n");
    int pos=(Get_Pos(net));
    printf("\n%d\n",pos);
    char c=Get_Char(pos);
    return c;


}

