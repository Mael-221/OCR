#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <SDL.h>
#include <SDL_image.h>

typedef struct VHistogram VHistogram; 
struct VHistogram
{
    int imgHeight;
    
    int elementNumber;

    float hist[];

};

typedef struct HorizontalHistogram HorizontalHistogram; 
struct HorizontalHistogram
{
    int imgWidth;
    
    int elementNumber;

    float hist[];

};

typedef struct line line; 
struct line
{
    int start;
    int end;
    int columnsNB;
};

typedef struct column column; 
struct column
{
    int start;
    int end;
};

typedef struct Letter Letter; 
struct Letter
{
    int TopX;
    int TopY;
    int BottomX;
    int BottomY;

    int Line;
};

typedef struct Iimage Iimage; 
struct Iimage
{
    SDL_Surface* image;

    int lineNumbers;

    struct Letter* Letters[];

};



int getMainColor(SDL_Surface* image);

int getPixelColor(SDL_Surface* image, Uint32 pixel);

struct VHistogram* createVHistogram(SDL_Surface* image);

struct HorizontalHistogram* createHHistogram(SDL_Surface* image, line li);

struct Iimage* createImage(SDL_Surface* image, SDL_Surface* debug);

line *divideInLines(SDL_Surface* i, int n,VHistogram* h, SDL_Surface* D);

column *divideInLetter(SDL_Surface* image, line li, int *nbLines,SDL_Surface* DebugImage);

int NumberOfLines(VHistogram* histogram,int h);

int NumberOfColumns(HorizontalHistogram* histogram,int w);

int lineSize(VHistogram* histogram, int start, int h);

int columnSize(HorizontalHistogram* histogram, int start, int w, float pixelLimit);

void fixGroups(SDL_Surface* image, line l,column* columns,SDL_Surface* debug);

Letter* mergeLetters(Letter* base, Letter* addition);


#endif