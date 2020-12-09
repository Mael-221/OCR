#include "reconstruct_char.h"

int Net_Pos(struct Neural_Network *net)
{
  double max = 0;
  int posMax = 0;

  for (int o = 0; o < net -> nbOutput; o++)
  {
    if (max < net -> OutputO[o])
    {
      posmax = o;
      max = net -> OutputO[o];
    }
  }
  return posmax;
}

char Net_Char(int val)
{
  char c;

  if(val <= 25)
  {
    c = val + 65;
  }
  else if(val > 25 && val <= 51)
  {
    c = (val + 97 - 26);
  }
  else if(val > 51 && val <= 61)
  {
    c = val + 48 - 52;
  }
  else
    {
      default:
      exit(1);
      break;
    }
  return c;
}

static void ForwardPass(struct Neural_Network *net)
{
  double sum;
  double weight;
  double output;
  double bias;

  //Calculate Output for Hidden neurons
  for (int h = 0; h < net -> nbHidden; h++)
  {
    sum = 0.0;
    for (int i = 0; i < net -> nbInput; i++)
    {
      weight = net -> WeightIH[i][h];
      output = net -> OutputI[i];

      sum += weight * output;
    }
    bias = net -> BiasH[h];
    net -> OutputH[h] = Sigmoid(sum + bias);
  }

  //Calculate Output for Output neurons
  for (int o = 0; o < net -> nbOutput; o++)
  {
    sum = 0.0;
    for (int h = 0; h < net -> nbHidden; h++)
    {
      weight = net -> WeightHO[h][o];
      output = net -> OutputH[h];

      sum += weight * output;
    }
    bias = net -> BiasO[o];
    net -> OutputO[o] = Sigmoid(sum + bias);
  }
}

char Reconstruct_Char(struct Neural_Network *net, double* letter)
{
    for(int i = 0; i < net -> nbInput; i++)
    {
        net -> OutputI[i] = letter[i];
    }

    ForwardPass(net);

    //Retrieve the character detected
    int pos = Net_Pos(net);
    char c = Net_Char(pos);

    return c;
}
