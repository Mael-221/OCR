#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <SDL.h>
#include <SDL_image.h>

typedef struct VerticalHistogram VerticalHistogram; 
struct VerticalHistogram
{
    int imgHeight;
    
    int elementNumber;

    float hist[];

};

typedef struct line line; 
struct line
{
    int start;
    int end;
};


int getMainColor(SDL_Surface* image);

int getPixelColor(SDL_Surface* image, Uint32 pixel);

struct VerticalHistogram* createVHistogram(SDL_Surface* image);

void divideLines(SDL_Surface* image);

int NumberOfLines(VerticalHistogram* histogram,int h);

int lineSize(VerticalHistogram* histogram, int start, int h);


#endif