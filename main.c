#include "Tools/tools.h"
#include "segmentation/histogram.h"
#include <stdio.h>


int main()
{
   
    SDL_Surface *image;

    image = load_image("images/s3_project_fr-1.png");

    divideLines(image);

    SDL_SaveBMP(image, "result.png");

    return 0;
}
