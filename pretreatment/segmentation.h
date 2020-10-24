#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>

struct image_matrix
{
  int* imp;
  size_t w;
  size_t h;
};

void line_cut(SDL_Surface* image);
void column_cut(SDL_Surface* image, int first_line, int end_line);

#endif
