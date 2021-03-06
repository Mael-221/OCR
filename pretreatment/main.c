#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "grayscale.h"
#include "binari.h"
#include <err.h>
#include "pixel_operations.h"
#include "contrast.h"
#include <math.h>
#include "noise.h"
#include "rotation.h"

//gcc $(sdl2-config --cflags --libs) -o prog *.c

void init_sdl()
{
    // Init only the video part.
    // If it fails, die with an error message.
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
}

SDL_Surface* display_image(SDL_Surface *img)
{
    SDL_Surface *screen;

    // Set the window to the same size as the image
    screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE|SDL_ANYFORMAT);
    if (screen == NULL)
    {
        // error management
        errx(1, "Couldn't set %dx%d video mode: %s\n",
                img->w, img->h, SDL_GetError());
    }

    // Blit onto the screen surface
    if(SDL_BlitSurface(img, NULL, screen, NULL) < 0)
        warnx("BlitSurface error: %s\n", SDL_GetError());

    // Update the screen
    SDL_UpdateRect(screen, 0, 0, img->w, img->h);

    // return the screen for further uses
    return screen;
}

SDL_Surface* load_image(char *path)
{
    SDL_Surface *img;

    // Load an image using SDL_image with format detection.
    // If it fails, die with an error message.
    img = IMG_Load(path);
    if (!img)
        errx(3, "can't load %s: %s", path, IMG_GetError());

    return img;
}


void wait_for_keypressed()
{
    SDL_Event event;

    // Wait for a key to be down.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYDOWN);

    // Wait for a key to be up.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYUP);
}

void update_surface(SDL_Surface* screen, SDL_Surface* image)
{
    if (SDL_BlitSurface(image, NULL, screen, NULL) < 0)
        warnx("BlitSurface error: %s\n", SDL_GetError());

    SDL_UpdateRect(screen, 0, 0, image->w, image->h);
}


int main()
{
  SDL_Surface *image;
  SDL_Surface *screen;
  init_sdl();

  image = load_image("images/image2.jpg");
  screen = display_image(image);
  wait_for_keypressed();
  grayscale(image);

  screen = display_image(image);
  wait_for_keypressed();

  
  image = rotation(image,45,0);
  screen = display_image(image);
  wait_for_keypressed();

  
  contrast(image,0.3333);
  update_surface(screen,image);
  wait_for_keypressed();

  contrast(image,3);
  update_surface(screen,image);
  wait_for_keypressed();
  
  noise(image);
  update_surface(screen,image);
  wait_for_keypressed();
  
  SDL_Quit();

  return EXIT_SUCCESS;
}
