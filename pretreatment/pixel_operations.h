#ifndef PIXEL_OPERATIONS_H
#define PIXEL_OPERATIONS_H

Uint32 getpixelval (SDL_Surface *image,int x , int y);

void putpixelval(SDL_Surface *image,int x, int y, Uint32 pixel);

#endif
