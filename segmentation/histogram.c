#include "histogram.h"
#include "../Tools/tools.h"
#include <stdio.h>

int getFontColor(SDL_Surface* image)
{
    int whitePixel = 0;
    int blackPixel = 0;

    for (int i = 0; i < image->w; i++)
    {
        for (int j = 0; j < image->h; j++)
        {
            Uint32 pixel = getpixelval(image,i,j);
	        Uint8 r , g ,b;
	        SDL_GetRGB(pixel,image->format,&r,&g,&b);

            if (r == 255)
            {
                whitePixel++;
            }
            else
            {
                blackPixel++;
            }
            
        }
    }

    if (whitePixel >= blackPixel)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int getPixelColor(SDL_Surface* image, Uint32 pixel)
{
	Uint8 r , g ,b;
	SDL_GetRGB(pixel,image->format,&r,&g,&b);

    if (r == 255)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

struct VerticalHistogram* createVHistogram(SDL_Surface* image)
{
    struct VerticalHistogram *histogram = malloc(sizeof(*histogram) + sizeof(int[image->h]));

    histogram->imgHeight = image->h;

    histogram->elementNumber = 0;

    int color = getFontColor(image);

    for (int i = 0; i < image->h; i++)
    {
        int c = 0;

        for (int j = 0; j < image->w; j++)
        {
            if (getPixelColor(image, getpixelval(image,j,i)) == color)
            {
                c++;
            }
        }
        histogram->hist[i] = ((float) c)/((float)image->w);
    }
    
    return histogram;
}

void divideLines(SDL_Surface* image)
{
    VerticalHistogram* histogram = createVHistogram(image);

    line lines[NumberOfLines(histogram,image->h)];

    int c = 0;

    int lineNumber = 0;

    while (c < image->h)
    {
        int taille = lineSize(histogram, c, image->h);


        if (histogram->hist[c] != 0)
        {
            histogram->elementNumber++;
            line l = {c,c+taille-1};
            lines[lineNumber] = l;
            lineNumber++;
        }
        
        c += taille;

    }

    

    for (int i = 0; i < histogram->elementNumber; i++)
    {
        SDL_Rect rtop;
        rtop.x = 0;
        rtop.y = lines[i].start;
        rtop.w = image->w;
        rtop.h = 1;
        
        SDL_Rect rbot;
        rbot.x = 0;
        rbot.y = lines[i].end;
        rbot.w = image->w;
        rbot.h = 1;


        SDL_FillRect(image,&rtop,SDL_MapRGB(image->format, 255,0,0));
        SDL_FillRect(image,&rbot,SDL_MapRGB(image->format, 255,0,0));

    }
    

}

int NumberOfLines(VerticalHistogram* histogram,int h)
{
    int c=0;

    int linenumbers = 0;

    while (c < h)
    {
        int taille = lineSize(histogram, c, h);
        if (histogram->hist[c] != 0)
        {
            linenumbers++;
        }
        c += taille;

    }
    return linenumbers;
}

int lineSize(VerticalHistogram* histogram, int start, int h)
{
    float startValue = histogram->hist[start];
    int c = 0; 

    if (startValue > 0)
    {
        while (histogram->hist[start + c] > 0 && start + c < h)
        {
            c++;
        }
    }
    else
    {
        while (histogram->hist[start + c] == 0 && start + c < h)
        {
            c++; 
        }
    }
    

    return c;
    
    
}