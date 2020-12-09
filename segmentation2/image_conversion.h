#ifndef IMAGE_CONVERSION_H
#define IMAGE_CONVERSION_H

#include <SDL.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>

struct image_matrix
{
  int* imp;
  size_t w;
  size_t h;
};

int* image_conversion(SDL_Surface* image);

#endif
