#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "pixel_operations.h"
#include <math.h>

Uint8 f(Uint8 c,double n)
{
  if(c<=127)
    {
      return (Uint8)((255/2)*(pow(2*c/255,n)));
    }
  else
    return 255 - f(255 - c,n);
}

void contrast_up(SDL_Surface* image)
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
	  float n = 1/3 ;
	  SDL_GetRGB(pixel,image->format,&r,&g,&b);
	  r = f(r,n);
	  n = 1/2 ;
	  g = f(g,n);
	  n = 2;
	  b = f(b,n);
	  pixel = SDL_MapRGB(image->format,r,g,b);
	  putpixelval(image,x,y,pixel);
	  y = y + 1;
	}
      x = x + 1;
    }  
}
