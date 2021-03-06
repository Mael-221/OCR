#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "binari.h"
#include "pixel_operations.h"

void binari(SDL_Surface *image)
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
	  Uint32 color = (r+g+b)/3;
	  if(color>=127)
	    {
	      pixel = SDL_MapRGB(image->format,255,255,255);
	      putpixelval(image,x,y,pixel);
	    }
	  else
	    {
	      pixel = SDL_MapRGB(image->format,0,0,0);
	      putpixelval(image,x,y,pixel);
	    }
	  y = y + 1;
	}
      x = x + 1;
    }
  
}
