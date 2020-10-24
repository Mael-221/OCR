#include "pixel_operations.h"
#include "segmentation.h"

void line_cut(SDL_Surface* image)
{
  Uint8 r, g, b;
  Uint32 pixel;
  int is_black = 0;
  int w = image -> w;
  int h = image -> h;
  int is_text_line = 0;
  int first_line;
  int end_line;
  for (int i = 0; i < h; i++)
    {
      for (int j = 0; j < w; j++)
	{
	  pixel = getpixelval(image, j, i);
	  SDL_GetRGB(pixel, image -> format, &r, &g, &b);
	  if (r != 255 || g != 255 || b != 255)
	    {
	      is_black = 1;
	      break;
	    }
	}
      if (is_black && !is_text_line)
	{
	  is_text_line = 1;
	  first_line = i;
	  int back_line = i - 1;
	  if (back_line >= 0)
	    {
	      for(int y = 0; y < w; y++)
		{
		  pixel = getpixelval(image, y, back_line);
		  pixel = SDL_MapRGB(image -> format, 255, 105, 180);
		  putpixelval(image, y, back_line, pixel);
		}
	      
	    }
	}
      if (!is_black && is_text_line)
	{
	  is_text_line = 0;
	  end_line = i;
	  for(int y = 0; y < w; y++)
	    {
	      pixel = getpixelval(image, y, i);
	      pixel = SDL_MapRGB(image -> format, 255, 105, 180);
	      putpixelval(image, y, i, pixel);
	    }
	  column_cut(image, first_line, end_line);
	}
      is_black = 0;
    }
}

void column_cut(SDL_Surface* image, int first_line, int end_line)
{
  Uint8 r, g, b;
  Uint32 pixel;
  int is_black = 0;
  int is_text_column = 0;
  int w = image -> w;
  for (int j = 0; j < w; j++)
    {
      for (int i = first_line; i < end_line; i++)
	{
	  pixel = getpixelval(image, j, i);
	  SDL_GetRGB(pixel, image -> format, &r, &g, &b);
	  if (r != 255 || g != 255 || b != 255)
	   {
	     is_black = 1;
	     break;
	   }
	}
      if (is_black && !is_text_column)
	{
	  is_text_column = 1;
	  int back_column = j - 1;
	  if (back_column >= 0)
	   {
	     for(int x = first_line; x < end_line; x++)
	       {
		 pixel = getpixelval(image, back_column, x);
		 pixel = SDL_MapRGB(image -> format, 255, 105, 180);
		 putpixelval(image, back_column, x, pixel);
	       }
	    }
	}
      if (!is_black && is_text_column)
	{
	  is_text_column = 0;
	  for(int x = first_line; x < end_line; x++)
	    {
	      pixel = getpixelval(image, j, x);
	      pixel = SDL_MapRGB(image -> format, 255, 105, 180);
	      putpixelval(image, j, x, pixel);
	     }
	 }
      
      is_black = 0;
    }
}
