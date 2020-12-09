#include "../Tools/tools.h"
#include "image_conversion.h"

int* image_conversion(SDL_Surface* image)
{
  Uint8 r, g, b;
  //signed numbers from 0 to 250, to register the rgb
  //components of a pixel
  Uint32 pixel;
  int w = image -> w;
  //register the width of the image
  int h = image -> h;
  //register the height of the image
  static int array[784];
  size_t k = 0;

  for (int i = 0; i < h; i++) //through lines
    {
      for (int j = 0; j < w; j++) //through columns
	{
	  pixel = getpixelval(image, j, i);
	  SDL_GetRGB(pixel, image -> format, &r, &g, &b);
	  //get the color of a pixel
	  if (r != 255 || g != 255 || b != 255)
	  //if the pixel is not white
	    {
	      array[k] = 1;
	    }
	  else
	    {
	      array[k] = 0;
	    }
	  k += 1;
	}
    }
  return array;
}
