#include "Ntools.h"

double sigmoid(double x)
{
    return 1.0/(1.0+exp(-x));
}
double derivative_sigmoid(double x)
{
    return x*(1.0 - x);
}
double Random()
{
    return ((double)rand()) / ((double)RAND_MAX)/10;
}

void print_matrix(double mat[], size_t lines, size_t cols)
{
    for(size_t i = 0; i < lines; i++)
    {
        for(size_t j = 0; j < cols; j++)
            printf("%d ", (int)mat[j + i * cols]);
        printf("\n");
    }
}

//Create & return the double* pixels values from filename
double *File_to_list(char *filename)
{
  double *list= malloc(sizeof(double) * 28 * 28);
  FILE *file = fopen(filename,"r");

  if(file == NULL)
    printf("File is NULL \n");
  for(int i = 0; i <= 28; i++)
  {
    for(int j = 0; j <= 28; j++)
    {
      int c = fgetc(file);
      if(c == 49)
        list[j+i*28] = 1;
      if(c == 48)
        list[j+i*28] = 0;
    }
  }
  fclose(file);
  return list;
}

//Create & return the double* goals pixels values from filename
double *goalArray(char letter)
{
  double *goal = malloc(52*sizeof(double));

  if(letter >= 'A' && letter <= 'Z')
    goal[(int)(letter) - 65] = 1;

  else if(letter >= 'a' && letter <= 'z')
    goal[((int)(letter) - 97) + 26] = 1;

  return goal;
}

//Create & return all the goals matrixes (for all letters)
double **goalMatrix()
{
  double **goalMatrix = malloc(sizeof(double*) * 52);
  char maj = 'A';
  char min = 'a';
  for(int i = 0; i < 52; i++)
  {
    if(i < 26)
    {
      goalMatrix[i] = goalArray(maj);
      maj++;
    }
    else
    {
      goalMatrix[i] = goalArray(min);
      min++;
    }
  }
  return goalMatrix;
}

//Create & return all the letters matrixes (for all letters)
double **lettersMatrix(char count)
{
  //Variables
  char majs_path[14] = "majs/0/00.txt\0";
  char mins_path[14] = "mins/0/00.txt\0";
  double **lettersMatrix = malloc(sizeof(double *) * 52);
  char maj = 'A';
  char min = 'a';
 
  for(int i = 0; i < 52; i++)
  {

    if(i < 26 )
    {
      majs_path[5] = maj;
      majs_path[7] = maj;
      majs_path[8] = count;
      lettersMatrix[i] = File_to_list(majs_path);
      maj++;

    }
    else if(i >= 26)
    {
      if(count==4)
          count ='3';
      mins_path[5] = min;
      mins_path[7] = min;
      mins_path[8] = count;
      lettersMatrix[i] = File_to_list(mins_path);
      min++;

    }
  }
  return lettersMatrix;
}

//Save data of the NN in 4 files:
//WeightIH - WeightHO - BiasH - BiasO
void save(struct Neural_Network *net)
{
  FILE* weightIH = fopen("weightIH.w", "w");
  for(int i = 0; i < net -> nbInput; ++i)
  {
    for(int h = 0; h < net -> nbHidden; ++h)
    {
      fprintf(weightIH, "%f\n", net -> WeightsIH[i][h]);
    }
  }
  fclose(weightIH);

  FILE* weightHO = fopen("weightHO.w", "w");
  for(int h = 0; h < net -> nbHidden; ++h)
  {
    for(int o = 0; o < net -> nbOutput; ++o)
    {
        fprintf(weightHO, "%f\n", net -> WeightsHO[h][o]);
    }
  }
  fclose(weightHO);

  FILE* biasH = fopen("biasH.b", "w");
  for(int h = 0; h < net -> nbHidden; ++h)
  {
    fprintf(biasH, "%f\n", net -> BiasH[h]);
  }
  fclose(biasH);

  FILE* biasO = fopen("biasO.b", "w");
  for (int o = 0; o < net -> nbOutput; ++o)
  {
    fprintf(biasO, "%f\n", net -> BiasO[o]);
  }
  fclose(biasO);
}

//Load a neural Netwwork previously saved in 4 files:
//WeightIH - WeightHO - BiasH - BiasO
struct Neural_Network* load ()
{
  //CREATE NN
  struct Neural_Network *net = malloc(sizeof(struct Neural_Network));
  net -> nbInput = 28*28; //size of imgs
  net -> nbHidden = 20;
  net -> nbOutput = 52; //26*2 letters
  

  int sizeMax = 15;
  char *line = calloc(15, sizeof(char));

  //WeightIH
  FILE* weightIH = fopen("weightIH.w", "r");
  for(int i = 0; i < net -> nbInput; ++i)
  {
    for(int h = 0; h < net -> nbHidden; ++h)
    {
      char *c = fgets(line, sizeMax, weightIH);
      if (c){};
      strtok(line, "\n");
      net -> WeightsIH[i][h] = atof(line);
    }
  }
  fclose(weightIH);

  //Weight HO
  FILE* weightHO = fopen("weightHO.w", "r");
  for(int h = 0; h < net -> nbHidden; ++h)
  {
    for(int o = 0; o < net -> nbOutput; ++o)
    {
        char *c = fgets(line, sizeMax, weightHO);
        if (c){};
        strtok(line, "\n");
        net -> WeightsHO[h][o] = atof(line);
    }
  }
  fclose(weightHO);

  //BiasH
  FILE* biasH = fopen("biasH.b", "r");
  for(int h = 0; h < net -> nbHidden; ++h)
  {
    char *c = fgets(line, sizeMax, biasH);
    if (c){};
    strtok(line, "\n");
    net -> BiasH[h] = atof(line);
  }
  fclose(biasH);

  //BiasO
  FILE* biasO = fopen("biasO.b", "r");
  for (int o = 0; o < net -> nbOutput; ++o)
  {
    char *c = fgets(line, sizeMax, biasO);
    if (c){};
    strtok(line, "\n");
    net -> BiasO[o] = atof(line);
  }
  fclose(biasO);

  return net;
}

//Return the position of the output with the greatest sigmoid result
int Get_Pos(struct Neural_Network *net)
{
  double max = 0;
  int posMax = 0;

  for (int o = 0; o < net -> nbOutput; o++)
  {
    if (max < net -> Outputs[o])
    {
      posMax = o;
      max = net -> Outputs[o];
    }
  }
  return posMax;
}

//Calculates the Squared error
void SquaredError(struct Neural_Network *net)
{
    double sum = 0;
    for (int o = 0; o < net -> nbOutput; o++)
    {
      sum += (net -> Goals[o] - net -> Outputs[o]) *
                      (net -> Goals[o] - net -> Outputs[o]);
    }
  net -> ErrorRate = 0.5 * sum;
}


//Retrive the position of the char in the goal tab
int Goal_Pos(double *goal)
{
  int c = 0;

  while(goal[c] != 1.0)
    c++;

  return c;
}

//Retrive char from value val
char Get_Char(int val)
{
  char c=0;

  if(val <= 25)
  {
    c = val + 65;
  }
  else if(val > 25 && val <= 51)
  {
    c = (val + 97 - 26);
  }

  return c;
}
