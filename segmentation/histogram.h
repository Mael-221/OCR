#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <SDL.h>
#include <SDL_image.h>
#include <gtk/gtk.h>

typedef enum charState charState; 
enum charState
{
    character,
    space,
    linebreak
};

/*
Histogram structure for VERTICAL image treatment
imgHeight : image height in pixel
elementNumber : Number of lines in the image 
hist[] : percentage array for each line
*/

typedef struct VHistogram VHistogram; 
struct VHistogram
{
    int imgHeight;
    
    int elementNumber;

    float hist[];
};

/*
Histogram structure for HORIZONTAL image treatment
imgWidth : image width in pixel
elementNumber : Number of columns in the line 
hist[] : percentage array for each column in the line
*/

typedef struct HHistogram HHistogram;
struct HHistogram
{
    int imgWidth;
    
    int elementNumber;

    float hist[];
};

/*
Line structure for line limit storage
start : y-value of line start
end : y-value of line end
columnsNB : numbers of column in the line
*/

typedef struct line line; 
struct line
{
    int start;
    int end;
    int columnsNB;
};

/*
Column structure for column limit storage
start : x-value of column start
end : x-value of column end
*/

typedef struct column column; 
struct column
{
    int start;
    int end;
    charState state;
};

/*
Letter structure to create zone ready for treatment
TopX : y-value of line start
TopY : y-value of line end
BottomX : x-value of column start
BottomY : x-value of column end
Line : line index to recover letter placement
*/

typedef struct Letter Letter; 
struct Letter
{
    int TopX;
    int TopY;
    int BottomX;
    int BottomY;

    int Line;
};


typedef struct Ichar Ichar; 
struct Ichar
{
    SDL_Surface* image;

    charState state;
};

typedef struct Iimage Iimage; 
struct Iimage
{
    SDL_Surface* image;

    int LetterNumbers;

    int color;

    Ichar Letters[10000];
};

/*
Return the least present color in the image.
*/
int getFontColor(Iimage* img);

/*
Return a 0 or 1 value from a 32 bits pixel color representation.
*/
int getPixelColor(SDL_Surface* image, Uint32 pixel);

/*
Return a line histogram from an image.
*/
struct VHistogram* createVHistogram(Iimage* img);

/*
Return a column histogram from an image.
*/
struct HHistogram* createHHistogram(Iimage* img, line li);

/*
Return a image structure for the image and draw debug segmentation on debug.
*/
struct Iimage* createImage(SDL_Surface* image, SDL_Surface* debug, int registerImage);

/*
Return a list of lines structures after segmentation.
*/
line *divideInLines(SDL_Surface* i, int n,VHistogram* h, SDL_Surface* D);

/*
Return a list of columns structures after segmentation.
*/
column *divideInLetter(Iimage* img, line l, int *nbL,SDL_Surface* DBG);

/*
Return the number of lines in the text.
*/
int NumberOfLines(VHistogram* histogram,int h);

/*
Return the number of columns in the text.
*/
int NumberOfColumns(HHistogram* histogram,int w);

/*
Return the size of a line from the start y value.
*/
int lineSize(VHistogram* histogram, int start, int h);

/*
Return the size of a column from the start x value.
*/
int columnSize(HHistogram* histogram, int start, int w, float pixelLimit);

/*
Fix potential groups of letter that are not segmented.
*/
void fixGroups(SDL_Surface* image, line l,column* columns,SDL_Surface* debug);

//Should not exist
Letter* mergeLetters(Letter* base, Letter* addition);

void Stretch_Linear(SDL_Surface* src,SDL_Surface* dest);

void reduceBlank(SDL_Surface* image, Letter* l);

SDL_Surface* Reduce(SDL_Surface* image, Letter* l);


#endif