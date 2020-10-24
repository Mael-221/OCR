#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "grayscale.h"
#include "binari.h"
#include <err.h>
#include "pixel_operations.h"
#include "contrast.h"
#include <math.h>
#include "noise.h"

void noise(SDL_Surface* image)
{
  int w = image -> w;
  int h = image -> h;
  int x = 0;
  while(x<w)
    {
      int y = 0;
      while(y<h)
	{
	  if(x==0)
	    {
	      if(y==0)
		{
		  //top left corner
		  Uint32 pixel = getpixelval(image,x,y);
		  Uint8 r , g ,b;
		  SDL_GetRGB(pixel,image->format,&r,&g,&b);
		  
		  Uint32 pixelxpy = getpixelval(image,x+1,y);
		  Uint8 rxpy , gxpy ,bxpy;
		  SDL_GetRGB(pixelxpy,image->format,&rxpy,&gxpy,&bxpy);
		  
		  Uint32 pixelxpyp = getpixelval(image,x+1,y+1);
		  Uint8 rxpyp , gxpyp ,bxpyp;
		  SDL_GetRGB(pixelxpyp,image->format,&rxpyp,&gxpyp,&bxpyp);
		  
		  Uint32 pixelxyp = getpixelval(image,x,y+1);
		  Uint8 rxyp , gxyp ,bxyp;
		  SDL_GetRGB(pixelxyp,image->format,&rxyp,&gxyp,&bxyp);
		  
		  pixel = SDL_MapRGB(image->format,(r+rxpy+rxpyp+rxyp)/4,(g+gxpy+gxpyp+gxyp)/4,(b+bxpy+bxpyp+bxyp)/4);//done
		  putpixelval(image,x,y,pixel);
		  
		}
	      else
		{
		  if(y==h-1)
		    {
		      //bottom left corner
		      Uint32 pixel = getpixelval(image,x,y);
		      Uint8 r , g ,b;
		      SDL_GetRGB(pixel,image->format,&r,&g,&b);
		      
		      Uint32 pixelxpy = getpixelval(image,x+1,y);
		      Uint8 rxpy , gxpy ,bxpy;
		      SDL_GetRGB(pixelxpy,image->format,&rxpy,&gxpy,&bxpy);
		      
		      Uint32 pixelxpym = getpixelval(image,x+1,y-1);
		      Uint8 rxpym , gxpym ,bxpym;
		      SDL_GetRGB(pixelxpym,image->format,&rxpym,&gxpym,&bxpym);
		      
		      Uint32 pixelxym = getpixelval(image,x,y-1);
		      Uint8 rxym , gxym ,bxym;
		      SDL_GetRGB(pixelxym,image->format,&rxym,&gxym,&bxym);
		      
		      pixel = SDL_MapRGB(image->format,(r+rxpy+rxpym+rxym)/4,(g+gxpy+gxpym+gxym)/4,(b+bxpy+bxpym+bxym)/4);//done
		      putpixelval(image,x,y,pixel);
		    }
		  else
		    {
		      //left side
		      Uint32 pixel = getpixelval(image,x,y);
		      Uint8 r , g ,b;
		      SDL_GetRGB(pixel,image->format,&r,&g,&b);
		      
		      Uint32 pixelxym = getpixelval(image,x,y-1);
		      Uint8 rxym , gxym ,bxym;
		      SDL_GetRGB(pixelxym,image->format,&rxym,&gxym,&bxym);
		      
		      Uint32 pixelxpym = getpixelval(image,x+1,y-1);
		      Uint8 rxpym , gxpym ,bxpym;
		      SDL_GetRGB(pixelxpym,image->format,&rxpym,&gxpym,&bxpym);
		      
		      Uint32 pixelxpy = getpixelval(image,x+1,y);
		      Uint8 rxpy , gxpy ,bxpy;
		      SDL_GetRGB(pixelxpy,image->format,&rxpy,&gxpy,&bxpy);
		      
		      Uint32 pixelxpyp = getpixelval(image,x+1,y+1);
		      Uint8 rxpyp , gxpyp ,bxpyp;
		      SDL_GetRGB(pixelxpyp,image->format,&rxpyp,&gxpyp,&bxpyp);
		      
		      Uint32 pixelxyp = getpixelval(image,x,y+1);
		      Uint8 rxyp , gxyp ,bxyp;
		      SDL_GetRGB(pixelxyp,image->format,&rxyp,&gxyp,&bxyp);
		      
		      pixel = SDL_MapRGB(image->format,((r+rxym+rxpym+rxpy+rxyp)/6),((g+gxym+gxpym+gxpy+gxyp)/6),((b+bxym+bxpym+bxpy+bxyp)/6));//done
	              putpixelval(image,x,y,pixel);
		    }
		}
	    }
	  else
	    {
	      if(x==w-1)
		{
		  if(y==0)
		    {
		      //top right corner
		      Uint32 pixel = getpixelval(image,x,y);
		      Uint8 r , g ,b;
		      SDL_GetRGB(pixel,image->format,&r,&g,&b);
		      
		      Uint32 pixelxmy = getpixelval(image,x-1,y);
		      Uint8 rxmy , gxmy ,bxmy;
		      SDL_GetRGB(pixelxmy,image->format,&rxmy,&gxmy,&bxmy);
		      
		      Uint32 pixelxmyp = getpixelval(image,x-1,y+1);
		      Uint8 rxmyp , gxmyp ,bxmyp;
		      SDL_GetRGB(pixelxmyp,image->format,&rxmyp,&gxmyp,&bxmyp);
		      
		      Uint32 pixelxyp = getpixelval(image,x,y+1);
		      Uint8 rxyp , gxyp ,bxyp;
		      SDL_GetRGB(pixelxyp,image->format,&rxyp,&gxyp,&bxyp);
		      
		      pixel = SDL_MapRGB(image->format,(r+rxmy+rxmyp+rxyp)/4,(g+gxmy+gxmyp+gxyp)/4,(b+bxmy+bxmyp+bxyp)/4);//done
		      putpixelval(image,x,y,pixel);
		    }
		  else
		    {
		      if(y==h-1)
			{
			  //bottom right corner
			  Uint32 pixel = getpixelval(image,x,y);
		          Uint8 r , g ,b;
		          SDL_GetRGB(pixel,image->format,&r,&g,&b);
			  
		          Uint32 pixelxmy = getpixelval(image,x-1,y);
		          Uint8 rxmy , gxmy ,bxmy;
		          SDL_GetRGB(pixelxmy,image->format,&rxmy,&gxmy,&bxmy);
			  
		          Uint32 pixelxmym = getpixelval(image,x-1,y-1);
		          Uint8 rxmym , gxmym ,bxmym;
		          SDL_GetRGB(pixelxmym,image->format,&rxmym,&gxmym,&bxmym);
			  
		          Uint32 pixelxym = getpixelval(image,x,y-1);
		          Uint8 rxym , gxym ,bxym;
		          SDL_GetRGB(pixelxym,image->format,&rxym,&gxym,&bxym);
			  
		          pixel = SDL_MapRGB(image->format,(r+rxmy+rxmym+rxym)/4,(g+gxmy+gxmym+gxym)/4,(b+bxmy+bxmym+bxym)/4);//done
		          putpixelval(image,x,y,pixel);
			}
		      else
			{
			  //right side
			  Uint32 pixel = getpixelval(image,x,y);
		          Uint8 r , g ,b;
		          SDL_GetRGB(pixel,image->format,&r,&g,&b);
			  
		          Uint32 pixelxym = getpixelval(image,x,y-1);
		          Uint8 rxym , gxym ,bxym;
		          SDL_GetRGB(pixelxym,image->format,&rxym,&gxym,&bxym);
			  
		          Uint32 pixelxmym = getpixelval(image,x-1,y-1);
		          Uint8 rxmym , gxmym ,bxmym;
		          SDL_GetRGB(pixelxmym,image->format,&rxmym,&gxmym,&bxmym);
			  
		          Uint32 pixelxmy = getpixelval(image,x-1,y);
		          Uint8 rxmy , gxmy ,bxmy;
		          SDL_GetRGB(pixelxmy,image->format,&rxmy,&gxmy,&bxmy);
			  
		          Uint32 pixelxmyp = getpixelval(image,x-1,y+1);
		          Uint8 rxmyp , gxmyp ,bxmyp;
		          SDL_GetRGB(pixelxmyp,image->format,&rxmyp,&gxmyp,&bxmyp);
			  
		          Uint32 pixelxyp = getpixelval(image,x,y+1);
		          Uint8 rxyp , gxyp ,bxyp;
		          SDL_GetRGB(pixelxyp,image->format,&rxyp,&gxyp,&bxyp);
			  
			  r = (r+rxym+rxmym+rxmy+rxmyp+rxyp)/6;
		          g = (g+gxym+gxmym+gxmy+rxmyp+gxyp)/6;
		          b = (b+bxym+bxmym+bxmy+bxmyp+bxyp)/6;
	      
	                  pixel = SDL_MapRGB(image->format,r,g,b); //done
	                  putpixelval(image,x,y,pixel);
			}
		    }
		}
	      else
		{
		  if(y==0)
		    {
		      //top line
		      Uint32 pixel = getpixelval(image,x,y);
		      Uint8 r , g ,b;
		      SDL_GetRGB(pixel,image->format,&r,&g,&b);
		      
		      Uint32 pixelxmy = getpixelval(image,x-1,y);
		      Uint8 rxmy , gxmy ,bxmy;
		      SDL_GetRGB(pixelxmy,image->format,&rxmy,&gxmy,&bxmy);

		      Uint32 pixelxmyp = getpixelval(image,x-1,y+1);
		      Uint8 rxmyp , gxmyp ,bxmyp;
		      SDL_GetRGB(pixelxmyp,image->format,&rxmyp,&gxmyp,&bxmyp);

		      Uint32 pixelxyp = getpixelval(image,x,y+1);
		      Uint8 rxyp , gxyp ,bxyp;
		      SDL_GetRGB(pixelxyp,image->format,&rxyp,&gxyp,&bxyp);

		      Uint32 pixelxpyp = getpixelval(image,x+1,y+1);
		      Uint8 rxpyp , gxpyp ,bxpyp;
		      SDL_GetRGB(pixelxpyp,image->format,&rxpyp,&gxpyp,&bxpyp);

		      Uint32 pixelxpy = getpixelval(image,x+1,y);
		      Uint8 rxpy , gxpy ,bxpy;
		      SDL_GetRGB(pixelxpy,image->format,&rxpy,&gxpy,&bxpy);

		      r = (r+rxmy+rxmyp+rxyp+rxpyp+rxpy)/6;
		      g = (g+gxmy+gxmyp+gxyp+rxpyp+gxpy)/6;
		      b = (b+bxmy+bxmyp+bxyp+bxpyp+bxpy)/6;

	              pixel = SDL_MapRGB(image->format,r,g,b); //done
	              putpixelval(image,x,y,pixel);
		    }
		  else
		    {
		      if(y==h-1)
			{
			  //bottom line
			  Uint32 pixel = getpixelval(image,x,y);
		          Uint8 r , g ,b;
		          SDL_GetRGB(pixel,image->format,&r,&g,&b);
		      
		          Uint32 pixelxmy = getpixelval(image,x-1,y);
		          Uint8 rxmy , gxmy ,bxmy;
		          SDL_GetRGB(pixelxmy,image->format,&rxmy,&gxmy,&bxmy);

		          Uint32 pixelxmym = getpixelval(image,x-1,y-1);
		          Uint8 rxmym , gxmym ,bxmym;
		          SDL_GetRGB(pixelxmym,image->format,&rxmym,&gxmym,&bxmym);

		          Uint32 pixelxym = getpixelval(image,x,y-1);
		          Uint8 rxym , gxym ,bxym;
		          SDL_GetRGB(pixelxym,image->format,&rxym,&gxym,&bxym);

		          Uint32 pixelxpym = getpixelval(image,x+1,y-1);
		          Uint8 rxpym , gxpym ,bxpym;
		          SDL_GetRGB(pixelxpym,image->format,&rxpym,&gxpym,&bxpym);

		          Uint32 pixelxpy = getpixelval(image,x+1,y);
		          Uint8 rxpy , gxpy ,bxpy;
		          SDL_GetRGB(pixelxpy,image->format,&rxpy,&gxpy,&bxpy);

		          r = (r+rxmy+rxmym+rxym+rxpym+rxpy)/6;
		          g = (g+gxmy+gxmym+gxym+rxpym+gxpy)/6;
                          b = (b+bxmy+bxmym+bxym+bxpym+bxpy)/6;

	                  pixel = SDL_MapRGB(image->format,r,g,b); //done
	                  putpixelval(image,x,y,pixel);
			}
		      else
			{
			  //everything else
			  Uint32 pixel = getpixelval(image,x,y);//
		          Uint8 r , g ,b;
		          SDL_GetRGB(pixel,image->format,&r,&g,&b);
		      
		          Uint32 pixelxmy = getpixelval(image,x-1,y);//
		          Uint8 rxmy , gxmy ,bxmy;
		          SDL_GetRGB(pixelxmy,image->format,&rxmy,&gxmy,&bxmy);

		          Uint32 pixelxmym = getpixelval(image,x-1,y-1);//
		          Uint8 rxmym , gxmym ,bxmym;
		          SDL_GetRGB(pixelxmym,image->format,&rxmym,&gxmym,&bxmym);

		          Uint32 pixelxym = getpixelval(image,x,y-1);//
		          Uint8 rxym , gxym ,bxym;
		          SDL_GetRGB(pixelxym,image->format,&rxym,&gxym,&bxym);

		          Uint32 pixelxpym = getpixelval(image,x+1,y-1);//
		          Uint8 rxpym , gxpym ,bxpym;
		          SDL_GetRGB(pixelxpym,image->format,&rxpym,&gxpym,&bxpym);

		          Uint32 pixelxpy = getpixelval(image,x+1,y);//
		          Uint8 rxpy , gxpy ,bxpy;
		          SDL_GetRGB(pixelxpy,image->format,&rxpy,&gxpy,&bxpy);

			  Uint32 pixelxpyp = getpixelval(image,x+1,y+1);//
		          Uint8 rxpyp , gxpyp ,bxpyp;
		          SDL_GetRGB(pixelxpyp,image->format,&rxpyp,&gxpyp,&bxpyp);

			  Uint32 pixelxyp = getpixelval(image,x,y+1);//
		          Uint8 rxyp , gxyp ,bxyp;
		          SDL_GetRGB(pixelxyp,image->format,&rxyp,&gxyp,&bxyp);

			  Uint32 pixelxmyp = getpixelval(image,x-1,y+1);//
		          Uint8 rxmyp , gxmyp ,bxmyp;
		          SDL_GetRGB(pixelxmyp,image->format,&rxmyp,&gxmyp,&bxmyp);
			  
			  r = (r+rxmy+rxmym+rxym+rxpym+rxpy+rxpyp+rxyp+rxmyp)/9;
			  
		          g = (g+gxmy+gxmym+gxym+rxpym+gxpy+bxpyp+bxyp+bxmyp)/9;
			 
		          b = (b+bxmy+bxmym+bxym+bxpym+bxpy+bxpyp+bxyp+bxmyp)/9;
			 
	                  pixel = SDL_MapRGB(image->format,(Uint8)r,(Uint8)g,(Uint8)b);
	                  putpixelval(image,x,y,pixel);
			}
		    }
		}
	    }
	  y = y + 1;
	}
      x = x + 1;
    }
}
