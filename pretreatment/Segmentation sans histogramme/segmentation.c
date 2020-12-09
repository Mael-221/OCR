#include "pixel_operations.h"
#include "segmentation.h"

void line_cut(SDL_Surface* image)
{
  Uint8 r, g, b;
  //signed numbers from 0 to 250, to register the rgb
  //components of a pixel
  Uint32 pixel;
  int is_black = 0;
  //check if a pixel is black
  //is_black set to false
  int w = image -> w;
  //register the width of the image
  int h = image -> h;
  //register the height of the image
  int is_text_line = 0;
  //check if we're on a text line
  //is_text_line set to false
  
  int first_line;
  //register the beginning of a text line
  int end_line;
  //register the end of a text line

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
	      is_black = 1;
	      //the pixel is black
	      break;
	    }
	}
      if (is_black && !is_text_line)
      //if the line has black pixels
	{
	  is_text_line = 1;
	  //we're on a text line
	  first_line = i;
	  //the beginning of the text line is registered
	  int back_line = i - 1;
	  //the previous line index is registered
	  if (back_line >= 0)
	  //if the previous line index is not negative
	    {
	      for(int y = 0; y < w; y++) //through columns
		{
		  pixel = getpixelval(image, y, back_line);
		  pixel = SDL_MapRGB(image -> format, 255, 105, 180);
		  //get the color of a pixel
		  //and set it to purple
		  putpixelval(image, y, back_line, pixel);
		  //change the color of
		  //the previous horizontal pixel line
		}
	      
	    }
	}
      if (!is_black && is_text_line)
      //if the previous line has black pixels but the current one not
	{
	  is_text_line = 0;
	  //we're not on a text line anymore
	  end_line = i;
	  //the end of the text line is registered
	  for(int y = 0; y < w; y++) //through columns
	    {
	      pixel = getpixelval(image, y, i);
	      pixel = SDL_MapRGB(image -> format, 255, 105, 180);
	      //get the color of a pixel
	      //and set it to purple
	      putpixelval(image, y, i, pixel);
	      //change the color of
	      //the current horizontal pixel line
	    }
	  column_cut(image, first_line, end_line);
	  //cut the characters of the current text line
	}
      is_black = 0;
      //is_black reset to false
    }
}

void column_cut(SDL_Surface* image, int first_line, int end_line)
{
  Uint8 r, g, b;
  //signed numbers from 0 to 250, to register the rgb
  //components of a pixel
  Uint32 pixel;
  int is_black = 0;
  //check if a column is sufficiently black
  //is_black set to false
  int is_white = 0;
  //check if a column is sufficiently white
  //is_white set to false
  int is_text_column = 0;
  //check if we're on a text column
  //is_text_line set to false
  int w = image -> w;
  //register the width of the image
  for (int j = 0; j < w; j++) //through columns
    {
      for (int i = first_line; i < end_line; i++) //through lines
	{
	  pixel = getpixelval(image, j, i);
	  SDL_GetRGB(pixel, image -> format, &r, &g, &b);
	  //get the color of a pixel
	  if (r != 255 || g != 255 || b != 255)
	  //if the pixel is not white
	   {
	     is_white += 1;
	     //we increment is_white
	   }
	}
      if (is_white < 3)
      //if the column checked is not sufficiently white
	{
	  is_black = 1;
	  //the column is from a character
	}
      if (is_black && !is_text_column)
      //if the column is from a character
	{
	  is_text_column = 1;
	  //we're on a text column
	  int back_column = j - 1;
	  //we register the index of the previous column
	  if (back_column >= 0)
	  //if the index of the previous column is not negative
	   {
	     for(int x = first_line; x < end_line; x++)
	     //through columns
	       {
		 pixel = getpixelval(image, back_column, x);
		 pixel = SDL_MapRGB(image -> format, 255, 105, 180);
                 //get the color of a pixel
		 //and set it to purple
		 putpixelval(image, back_column, x, pixel);
		 //change de color of
		 //the previous vertical pixel line
	       }
	    }
	}
      if (!is_black && is_text_column)
      //if the current column is not from a character
      //but the previous one yes
	{
	  is_text_column = 0;
	  //we're not on a text column anymore
	  for(int x = first_line; x < end_line; x++)
	  //through columns
	    {
	      pixel = getpixelval(image, j, x);
	      pixel = SDL_MapRGB(image -> format, 255, 105, 180);
	      //get the color of a pixel
	      //and set it to purple
	      putpixelval(image, j, x, pixel);
	      //change the color of
	      //the current vertical pixel line
	     }
	 }
      is_white = 0;
      //is_white reset to false
      is_black = 0;
      //is_black reset to false 
    }
}
