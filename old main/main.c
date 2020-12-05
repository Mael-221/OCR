#include "Tools/tools.h"
#include "segmentation/histogram.h"
#include <stdio.h>
#include "pretreatment/binari.h"
#include "pretreatment/grayscale.h"
#include "pretreatment/contrast.h"
#include "pretreatment/noise.h"
#include <string.h>
#include "segmentation2/segmentation.h"
#include "NeuralNetwork/NN_xor.h"


/*int main()
{
   
    SDL_Surface *image;
    SDL_Surface *debug;

    image = load_image("images/zebi.png");

    debug = load_image("images/zebi.png");

    contrast_up(image);

    grayscale(image);

    noise(image);

    binari(image);

    contrast_up(debug);

    grayscale(debug);

    noise(debug);

    binari(debug);
    

    createImage(image,debug);
    
    SDL_SaveBMP(debug, "result.png");

    return 0;
}*/


//Compare two string
int STRCMP (const char *p1, const char *p2)
{
  const unsigned char *s1 = (const unsigned char *) p1;
  const unsigned char *s2 = (const unsigned char *) p2;
  unsigned char c1, c2;
  do
    {
      c1 = (unsigned char) *s1++;
      c2 = (unsigned char) *s2++;
      if (c1 == '\0')
        return c1 - c2;
    }
  while (c1 == c2);
  return c1 - c2;
}

int main(int argc, char const *argv[])
{  
    if (argc >= 1)
    {
        //Initializing the image for treatment and a debug image for segmentation.
        SDL_Surface *image;
        SDL_Surface *debug;

        const char* arg = argv[1];

        arg += 1;

        image = load_image(argv[1]);

        debug = load_image(argv[1]);

        for (int i = 2; i < argc; i++)
        {

            if (STRCMP(argv[i],"-bin") == 0 || STRCMP(argv[i],"-b") == 0)
            {
                binari(image);
                binari(debug);
            }

            if (STRCMP(argv[i],"-contrastup") == 0 || STRCMP(argv[i],"-cup") == 0)
            {
                contrast(image,3);
                contrast(debug,3);
            }

            if (STRCMP(argv[i],"-contrastdown") == 0 || STRCMP(argv[i],"-cdown") == 0)
            {
                contrast(image,0.3);
                contrast(debug,0.3);
            }

            if (STRCMP(argv[i],"-grayscale") == 0|| STRCMP(argv[i],"-g") == 0)
            {
                grayscale(image);
                grayscale(debug);
            }

            if (STRCMP(argv[i],"-noise") == 0|| STRCMP(argv[i],"-n") == 0)
            {
                noise(image);
                noise(debug);
            }

            if (STRCMP(argv[i],"-wait") == 0|| STRCMP(argv[i],"-w") == 0)
            {
                wait_for_keypressed();
            }

            if (STRCMP(argv[i],"-seg") == 0|| STRCMP(argv[i],"-s") == 0)
            {
                createImage(image,debug);
            }

            if (STRCMP(argv[i],"-seg2") == 0|| STRCMP(argv[i],"-s2") == 0)
            {
                line_cut(debug);
            }

            if (STRCMP(argv[i],"-XOR") == 0)
            {
                XOR();
            }

            if (STRCMP(argv[i],"-present") == 0)
            {
                //for the presentation
            }

            if (STRCMP(argv[i],"-line") == 0)
            {
                VHistogram* lineHist = createVHistogram(image);

                int numberOfLines = NumberOfLines(lineHist, image->h);

                divideInLines(image,numberOfLines,lineHist,debug);
            }

            if (STRCMP(argv[i], "-show") == 0)
            {
                init_sdl();
                display_image(debug);
                wait_for_keypressed();
                SDL_Quit();
            }
              
        }

        SDL_SaveBMP(debug, "images/result.png");

        return 0;
    }
    else
    {
        return 1;
    }
}

