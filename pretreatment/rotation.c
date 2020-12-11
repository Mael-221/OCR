#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "pixel_operations.h"
#include <math.h>

SDL_Surface* rotation(SDL_Surface* image,int de,int boo)
{
  if(de>180)
    de = de - 180;
  double deg = de*0.0174533;
  int width = image -> w;
  int height = image -> h;
  int Rwidth = 0;
  int Rheight = 0;
  if(boo == 0)
    {
      Rwidth = sqrt(width*width+height*height);
      Rheight = sqrt(width*width+height*height);
    }
  else
    {
      Rwidth = width;
      Rheight = height;
    }
  SDL_Surface *rotatedimage;
  rotatedimage = SDL_CreateRGBSurface(0,Rwidth,Rheight,32,0,0,0,0);
  
  
  int x = 0;
  while(x<Rwidth)
    {
      int y = 0;
      while(y<Rheight)
	{
	  Uint32 pixel = SDL_MapRGB(rotatedimage->format,0,0,0);
	  putpixelval(rotatedimage,x,y,pixel);
	  y = y + 1;
	}
      x = x + 1;
    }
  
  
  x = 0;
  while(x<width)
    {
      int y = 0;
      while(y<height)
	{
	  Uint32 pixel = getpixelval(image,x,y);
	  double x2 = 0;
	  double y2 = 0;
	  x2 = cos(deg)*(x-(width/2)) + sin(deg)*(y-(height/2)) + Rwidth/2;
	  y2 = -sin(deg)*(x-(width/2))+cos(deg)*(y-(height/2)) + Rheight/2;
	  if(x2<0)
	    x2 = Rwidth+x2;
	  if(y2<0)
	    y2 = Rheight+y2;
	  if(x2<Rwidth && y2<Rheight)
	    {
	      putpixelval(rotatedimage,x2,y2,pixel);
	    }
	  	
	  y = y + 1;
      	}
      x = x + 1;
    }
  
  return rotatedimage;
}
