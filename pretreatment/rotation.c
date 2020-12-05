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
      Rheight = sqrt(width*width+height*height);//- 2*(width*height*sin(deg)
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
	  //printf("done in %i , %i \n",x,y);
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
	  //Uint32 pixel = SDL_MapRGB(rotatedimage->format,150,5,0);
	  Uint32 pixel = getpixelval(image,x,y);
	  //Uint8 r , g ,b;
	  //SDL_GetRGB(pixel,image->format,&r,&g,&b)
	  //putpixelval(rotatedimage,x,y,pixel);
	  double x2 = 0;
	  double y2 = 0;
	  if((de >= 45 && de<=90))
	    x2 =height*sin(deg)+(x)*cos(deg) - (y)*sin(deg);
	  else
	    x2 = height*sin(deg) + (x)*cos(deg) - (y)*sin(deg);

	  if(de>110)
	    x2 =(x)*cos(deg) - (y)*sin(deg);

	  
	  if(de > 90)
	    y2 =-height*cos(deg)+(y)*cos(deg) + (x)*sin(deg);
	  else
	     y2 =(y)*cos(deg) + (x)*sin(deg);
	  //printf("tried in %i, %i to %f %f \n",x,y,x2,y2);
	  if(x2<0)
	    x2 = Rwidth+x2;
	  if(y2<0)
	    y2 = Rheight+y2;
	  if(x2<Rwidth && y2<Rheight)
	    {
	      putpixelval(rotatedimage,x2,y2,pixel);
	      //printf("done in %i, %i to %f %f \n",x,y,x2,y2);
	    }
	  	
	  y = y + 1;
      	}
      x = x + 1;
    }
  
  return rotatedimage;
}
