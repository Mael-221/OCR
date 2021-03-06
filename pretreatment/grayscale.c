#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "grayscale.h"
#include "pixel_operations.h"

void grayscale(SDL_Surface* image)
{
  int w = image -> w;
  int h = image -> h;
  int x = 0;
  while(x<w)
    {
      int y = 0;
      while(y<h)
	{
	  Uint32 pixel = getpixelval(image,x,y);
	  Uint8 r , g ,b;
	  SDL_GetRGB(pixel,image->format,&r,&g,&b);
	  Uint32 grey = 0.3*r + 0.59*g + 0.11*b;
	  pixel = SDL_MapRGB(image->format,grey,grey,grey);
	  putpixelval(image,x,y,pixel);
	  y = y + 1;
	}
      x = x + 1;
    }
  
}
