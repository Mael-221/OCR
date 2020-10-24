#ifndef TOOLS_H
#define TOOLS_H

#include <SDL.h>
#include <SDL_image.h>

void init_sdl();

SDL_Surface* display_image(SDL_Surface *img);

SDL_Surface* load_image(const char *path);

void wait_for_keypressed();

void update_surface(SDL_Surface* screen, SDL_Surface* image);

Uint32 getpixelval(SDL_Surface *image, int x, int y);
void putpixelval(SDL_Surface *surface, int x, int y, Uint32 pixel);

#endif