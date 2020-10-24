#include "histogram.h"
#include "../Tools/tools.h"
#include <stdio.h>

//Return 0 for majority of White pixels and 1 for majority of Black pixels
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

//Return if a pixel is Black or White (1 for white, 0 for black)
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

struct VHistogram* createVHistogram(SDL_Surface* image)
{
    struct VHistogram *histogram = malloc(sizeof(*histogram) + sizeof(int[image->h]));

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

struct HorizontalHistogram* createHHistogram(SDL_Surface* image, line li)
{
    struct HorizontalHistogram *histogram = malloc(sizeof(*histogram) + sizeof(int[image->w]));

    histogram->imgWidth = image->w;

    histogram->elementNumber = 0;

    int color = getFontColor(image);

    for (int i = 0; i < image->w; i++)
    {
        int c = 0;

        for (int j = (li.start); j < li.end; j++)
        {
            if (getPixelColor(image, getpixelval(image,i,j)) == color)
            {
                c++;
            }
        }
        histogram->hist[i] = ((float) c)/((float)(li.end-li.start));
    }
    
    return histogram;
}

struct Iimage* createImage(SDL_Surface* image, SDL_Surface* debug)
{
    VHistogram* lineHist = createVHistogram(image);
    int numberOfLines = NumberOfLines(lineHist, image->h);

    line* lines = divideInLines(image,numberOfLines,lineHist,debug);

    Letter* letters = malloc(sizeof(Letter*));

    for (int i = 0; i < numberOfLines; i++)
    {
        int nbColumns = 0;
        column* columns = divideInLetter(image,lines[i],&nbColumns,debug);

        lines[i].columnsNB = nbColumns;
        fixGroups(image, lines[i], columns,debug);

        //letters = mergeLetters(letters, temp);

    }
    

    Iimage* img = malloc(sizeof(Iimage) + sizeof(letters));

    img->image = image;
    img->lineNumbers = numberOfLines;

    return img;
}

line *divideInLines(SDL_Surface* image, int nbLines, VHistogram* histogram, SDL_Surface* DebugImage)
{

    line* lines = malloc(nbLines * sizeof(line));

    int c = 0;

    int lineNumber = 0;

    while (c < image->h)
    {
        int taille = lineSize(histogram, c, image->h);

        if (histogram->hist[c] != 0)
        {
            histogram->elementNumber++;
            line l = {c,c+taille-1,0};
            lines[lineNumber] = l;
            lineNumber++;
        }
        
        c += taille;
    }

    if (DebugImage)
    {
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

            SDL_FillRect(DebugImage,&rtop,SDL_MapRGB(DebugImage->format, 255,0,0));
            SDL_FillRect(DebugImage,&rbot,SDL_MapRGB(DebugImage->format, 255,0,0));
        }
    }

    return lines;
}

column *divideInLetter(SDL_Surface* image, line li,int* nb, SDL_Surface* DebugImage)
{
    HorizontalHistogram* histogram = createHHistogram(image,li);

    column* columns = malloc(NumberOfColumns(histogram,image->w) * sizeof(column));

    int c = 0;

    int columnNumber = 0;

    while (c < image->w)
    {
        
        int taille = columnSize(histogram, c, image->w,0);

        if (histogram->hist[c] != 0)
        {
            histogram->elementNumber++;
            column l = {c,c+taille-1};
            columns[columnNumber] = l;
            columnNumber++;
        }
        
        c += taille;
    }

    if (DebugImage)
    {
        for (int i = 0; i < histogram->elementNumber; i++)
        {
            SDL_Rect rleft;
            rleft.x = columns[i].start;
            rleft.y = li.start;
            rleft.w = 1;
            rleft.h = li.end-li.start;
        
            SDL_Rect rright;
            rright.x = columns[i].end;
            rright.y = li.start;
            rright.w = 1;
            rright.h = li.end-li.start;

            SDL_FillRect(DebugImage,&rleft,SDL_MapRGB(DebugImage->format, 255,0,0));
            SDL_FillRect(DebugImage,&rright,SDL_MapRGB(DebugImage->format, 255,0,0));
        }
    }

    *nb = columnNumber;

    return columns;
}


int NumberOfLines(VHistogram* histogram,int h)
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

int NumberOfColumns(HorizontalHistogram* histogram,int w)
{
    int c=0;

    int columnNumber = 0;

    while (c < w)
    {
        int taille = columnSize(histogram, c, w,0);
        if (histogram->hist[c] != 0)
        {
            columnNumber++;
        }
        c += taille;

    }
    return columnNumber;
}

int lineSize(VHistogram* histogram, int start, int h)
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

int columnSize(HorizontalHistogram* histogram, int start, int w, float pixelLimit)
{
    float startValue = histogram->hist[start];
    int c = 0; 

    if (pixelLimit != 0)
    {
        pixelLimit -= 0.000001;
    }
    

    if (startValue > 0)
    {
        while (histogram->hist[start + c] > pixelLimit && start + c < w)
        {
            c++;
        }
    }
    else
    {
        while (histogram->hist[start + c] <= pixelLimit && start + c < w)
        {
            c++; 
        }
    }
    return c;
}

void fixGroups(SDL_Surface* image, line l,column* columns, SDL_Surface* debug)
{
    if(image && columns && debug)
    {
        l.start = l.start;
    }

    /*
    float m = 0;
    for (int i = 0; i < l.columnsNB; i++)
    {
        m += columns[i].end - columns[i].start + 1;
    }
    m = (m/(l.columnsNB))*1.5;

    for (int i = 0; i < l.columnsNB; i++)
    {
      
        if ((columns[i].end - columns[i].start) > m)
        {
            SDL_Rect r;
            r.x = columns[i].start;
            r.y = l.start;
            r.w = 1;
            r.h = l.end-l.start;
        
            SDL_FillRect(debug,&r,SDL_MapRGB(debug->format, 0,0,255));


            int count = 1;
            while (count < 4 )
            {
                HorizontalHistogram* hist = createHHistogram(image,l);

                int c = columnSize(hist, columns[i].start, columns[i].end , count /((float)(l.end-l.start)));

                //printf("c:%d    start:%f  end:%d    limit:%f\n",c,hist->hist[columns[i].start],columns[i].end - columns[i].start,count /((float)(l.end-l.start)));
            
                if (c == columns[i].end - columns[i].start || c== 0)
                {
                    if(c == 0)
                    {count = 4;}
                    else
                    {
                        count++;
                    }
                
                }
                else
                {
                    count = 1;
                    columns[i].start = columns[i].start +c;

                    SDL_Rect r;
                    r.x = columns[i].start;
                    r.y = l.start;
                    r.w = 1;
                    r.h = l.end-l.start;
        
                    SDL_FillRect(debug,&r,SDL_MapRGB(debug->format, 0,255,0));
                }
            }    
        }
        
    }
    */
}

Letter* mergeLetters(Letter* base, Letter* addition)
{
    Letter* result = malloc(sizeof(base) + sizeof(addition));

    int c = 0;
    for (unsigned long i = 0; i < sizeof(base)/sizeof(Letter*); i++)
    {
        result[i] = base[i];
        c++;
    }

    for (unsigned long i = 0; i < sizeof(addition)/sizeof(Letter*); i++)
    {
        result[i + c] = addition[i];
    }
    
    return result;
}