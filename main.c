#include "Tools/tools.h"
#include "segmentation/histogram.h"
#include <stdio.h>


int main()
{
   
    SDL_Surface *screen;
    SDL_Surface *image;

    init_sdl();

    image = load_image("images/s3_project_fr-1.png");
    screen = display_image(image);
    wait_for_keypressed();

    //VerticalHistogram *histogram = 
    divideLines(image);

    update_surface(screen,image);

    wait_for_keypressed();

    for (int i = 0; i < image->h; i++)
    {
        //printf("%f\n", histogram->hist[i]);
    }

    SDL_Quit();

    return 0;
}
