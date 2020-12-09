#include "histogram.h"
#include "../Tools/tools.h"
#include "../pretreatment/binari.h"
#include <stdio.h>

//Return 0 for majority of White pixels and 1 for majority of Black pixels
int getFontColor(Iimage* img)
{
    //
    SDL_Surface* image = img->image;

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

struct VHistogram* createVHistogram(Iimage* img)
{
    SDL_Surface* image = img->image;

    //allocate memory for the array in the struct.
    struct VHistogram *hist = malloc(sizeof(*hist) + sizeof(int[image->h]));

    hist->imgHeight = image->h;

    hist->elementNumber = 0;

    //Get the color of text (0 for black and 1 for white)
    img->color = getFontColor(img);
    int color = img->color;

    for (int i = 0; i < image->h; i++)
    {
        int c = 0;

        for (int j = 0; j < image->w; j++)
        {
            //Count the number of pixel of the text color in the pixel line.
            if (getPixelColor(image, getpixelval(image,j,i)) == color)
            {
                c++;
            }
        }
        //Make a percentage of text in the line.
        hist->hist[i] = ((float) c)/((float)image->w);
    }
    
    return hist;
}

struct HHistogram* createHHistogram(Iimage* img, line li)
{
    SDL_Surface* image = img->image;

    //allocate memory for the array in the struct.
    struct HHistogram *hist = malloc(sizeof(*hist) + sizeof(int[image->w]));

    hist->imgWidth = image->w;

    hist->elementNumber = 0;

    int color = img->color;

    for (int i = 0; i < image->w; i++)
    {
        int c = 0;

        for (int j = (li.start); j < li.end; j++)
        {
            //Count the number of pixel of the text color in the pixel line.
            if (getPixelColor(image, getpixelval(image,i,j)) == color)
            {
                c++;
            }
        }
        //Make a percentage of text in the line.
        hist->hist[i] = ((float) c)/((float)(li.end-li.start));
    }
    
    return hist;
}

//Complete segmentation
struct Iimage* createImage(SDL_Surface* image, SDL_Surface* debug, int registerImage)
{
    Iimage* img = malloc(sizeof(Iimage));

    img->image = image; // Set image reference in struct

    Ichar* Letters = img->Letters;

    //Variables to segment the text in lines.
    VHistogram* lineHist = createVHistogram(img);
    int numberOfLines = NumberOfLines(lineHist, image->h);

    line* lines = divideInLines(image,numberOfLines,lineHist,debug);


    int LetterCount = 0;
    for (int i = 0; i < numberOfLines; i++)
    {
        
        int nbColumns = 0;
        column* columns = divideInLetter(img,lines[i],&nbColumns,debug);

        lines[i].columnsNB = nbColumns;

        for (int j = 0; j < lines[i].columnsNB; j++)
        {
            Letter let = {columns[j].start, lines[i].start, columns[j].end, lines[i].end,i};
            
            //int newH = let.BottomY - let.TopY;
            //int newW = let.BottomX - let.TopX;

            //g_print("W:%d H:%d\n",newW,newH);
            /**
            switch (columns[j].state)
            {
            case character:
                g_print("x");
                break;
            case linebreak:
                g_print("/");
                break;

            case space:
                g_print("_");
                break;
            
            default:
                break;
            }
            **/
            if (columns[j].state == character)
            {
                reduceBlank(image, &let);
            
                SDL_Surface* img = Reduce(image, &let);

                binari(img);

                /**
            newH = let.BottomY - let.TopY;
            newW = let.BottomX - let.TopX;

            SDL_Surface* img = SDL_CreateRGBSurface(0,newW,newH,32,0,0,0,0);

            for (int i = 0; i < newW; i++)
            {
                for (int j = 0; j < newH; j++)
                {
                    putpixelval(img,i,j,getpixelval(image,let.TopX + i, let.TopY + j));
                }
            }
            **/

                if (registerImage > 0)
                {
                    char buf[128];
                    int i = 0;

                    if (LetterCount + 65 <= 90)
                    {
                        i = 65;
                        snprintf(buf, 128, "NeuralNetwork/majs/%c/%c%d.txt", (char) (i + LetterCount),(char) (i + LetterCount),registerImage);
                    }
                    else
                    {
                        i = 70;
                        snprintf(buf, 128, "NeuralNetwork/mins/%c/%c%d.txt", (char) (i + LetterCount),(char) (i + LetterCount),registerImage);
                    }

                    //g_print("W:%d\nH:%d\n", img->w, img->h);

                    FILE *file = fopen(buf,"w");

                    for (int i = 0; i < img->w; i++)
                    {
                        for (int j = 0; j < img->h; j++)
                        {
                            //g_print("%d\n",getPixelColor(img,getpixelval(img,i,j)));

                            if (getpixelval(img,j,i) == 0)
                            {
                                fprintf(file,"%c",'1');
                                //g_print("%c",'0');
                            }
                            else
                            {
                                fprintf(file,"%c",'0');
                                //g_print("%c",'1');
                            }

                            
                        }
                        fprintf(file,"\n");
                    }

                    fclose(file);
                    
                }
            
                Ichar ch = {
                    img,
                    character
                };

                Letters[LetterCount] = ch;

                //g_print("W:%d H:%d\n",let.BottomX - let.TopX,let.BottomY - let.TopY);
                LetterCount++;
            }

            else
            {
                if (columns[j].state == space)
                {
                    Ichar ch = {
                        NULL,
                        space
                    };

                    Letters[LetterCount] = ch;
                }
                else
                {
                    Ichar ch = {
                        NULL,
                        linebreak
                    };

                    Letters[LetterCount] = ch;
                }
                LetterCount++;
            }
            
        }
        
    }
    
    img->image = image;
    img->LetterNumbers = LetterCount;

    return img;
}

//Reduce White pixel at top and bottom of letters
void reduceBlank(SDL_Surface* image, Letter* l)
{
    int IsHighMinimal = 0;
    int isLowMinimal = 0;

    //g_print("TopX:%d\nTopY:%d\nBottomX:%d\nBottomY:%d\n",l->TopX,l->TopY,l->BottomX,l->BottomY);

    while (IsHighMinimal == 0)
    {
        int count = 0;
        for (int i = 0; i < l->BottomX - l->TopX; i++)
        {
            if (getPixelColor(image,getpixelval(image,l->TopX + i, l->TopY)) == 0)
            {
                count++;
            }
        }
        if (count == 0)
        {
            l->TopY++;
            //g_print("TopX:%d\nTopY:%d\nBottomX:%d\nBottomY:%d\n///++///\n",l->TopX,l->TopY,l->BottomX,l->BottomY);
        }
        else
        {
            IsHighMinimal = 1;
        } 
    }
    
    while (isLowMinimal == 0)
    {
        int count = 0;
        for (int i = 0; i < l->BottomX - l->TopX; i++)
        {
            if (getPixelColor(image,getpixelval(image,l->TopX + i, l->BottomY)) == 0)
            {
                count++;
            }
        }
        if (count == 0)
        {
            l->BottomY--;
            //g_print("TopX:%d\nTopY:%d\nBottomX:%d\nBottomY:%d\n///--///\n",l->TopX,l->TopY,l->BottomX,l->BottomY);
        }
        else
        {
            isLowMinimal = 1;
        }
    }
}

//Reduce to image of 28*28 pixels
SDL_Surface* Reduce(SDL_Surface* image, Letter* l)
{
    int newH = l->BottomY - l->TopY; //X value to iterate
    int newW = l->BottomX - l->TopX; //Y Value to iterate

    SDL_Surface* finaldest = SDL_CreateRGBSurface(0,28,28,32,0,0,0,0); //Final image of 28*28

    //If source image is under 28*28 on a axis we adjust it
    if (newH < 29)
    {
        newH = 29;
    }
    if (newW < 29)
    {
        newW = 29;
    }

    SDL_Surface* dest = SDL_CreateRGBSurface(0,newW,newH,32,0,0,0,0);

    //Draw a white rectangle to fill potential new image part created higher
    SDL_Rect r;
    r.x = 0;
    r.y = 0;
    r.w = dest->w;
    r.h = dest->h;

    SDL_FillRect(dest,&r,SDL_MapRGB(dest->format, 255,255,255));

    //Copy source image to temp image of at least 28+*28+
    for (int i = 0; i < l->BottomX - l->TopX; i++)
        {
        for (int j = 0; j < l->BottomY - l->TopY; j++)
            {
                putpixelval(dest,i,j,getpixelval(image,l->TopX + i, l->TopY + j));
            }
        }

    //Reduce to 28*28
    Stretch_Linear(dest,finaldest);

    newH = 28;
    newW = 28;
    
    return finaldest;
}

line *divideInLines(SDL_Surface* img, int nbL, VHistogram* h, SDL_Surface* Dbg)
{
    //Allocate memory for the array in the struct.
    line* lines = malloc(nbL * sizeof(line));

    int c = 0;

    int lineNumber = 0;

    while (c < img->h)
    {
        int taille = lineSize(h, c, img->h);

        if (h->hist[c] != 0)
        {
            h->elementNumber++;
            line l = {c,c+taille-1,0};
            lines[lineNumber] = l;
            lineNumber++;
        }
        
        c += taille;
    }

    if (Dbg)
    {
        for (int i = 0; i < h->elementNumber; i++)
        {
            //Draw debug lines for a clear representation

            SDL_Rect rtop;
            rtop.x = 0;
            rtop.y = lines[i].start;
            rtop.w = img->w;
            rtop.h = 1;
        
            SDL_Rect rbot;
            rbot.x = 0;
            rbot.y = lines[i].end;
            rbot.w = img->w;
            rbot.h = 1;

            SDL_FillRect(Dbg,&rtop,SDL_MapRGB(Dbg->format, 255,0,0));
            SDL_FillRect(Dbg,&rbot,SDL_MapRGB(Dbg->format, 255,0,0));
        }
    }

    return lines;
}

column *divideInLetter(Iimage* img, line li,int* nb, SDL_Surface* Dbg)
{
    SDL_Surface* image = img->image;

    HHistogram* hist = createHHistogram(img,li);

    column* columns = malloc(NumberOfColumns(hist,image->w) * sizeof(column));

    int c = 0;

    int columnNumber = 0;
    int letterNumber = 0;
    int sum = 0;
    float moy = 0;

    while (c < image->w)
    {
        
        int taille = columnSize(hist, c, image->w,0);

        if (hist->hist[c] != 0)
        {
            hist->elementNumber++;
            column l = {c,c+taille-1,character};
            columns[columnNumber] = l;
            columnNumber++;
            letterNumber++;
            sum += taille;
            moy = (float) sum / (float) letterNumber;
        }

        if (hist->hist[c] == 0 && c != 0)
        {
            if (taille > (moy) * ((float) 3 / (float) 4))
            {
                //g_print("Taille: %d, Limit:%f\n",taille, (moy) * ((float) 3 / (float) 4));

                if (c + taille == image->w)
                {
                    //g_print("end\n");

                    column l = {0,0,linebreak};
                    columns[columnNumber] = l;
                    //g_print("/");
                }
                else
                {
                    column l = {0,0,space};
                    columns[columnNumber] = l;
                    //g_print("_");
                }

                columnNumber++;

            }
        }

        c += taille;
    }

    if (Dbg)
    {
        for (int i = 0; i < columnNumber; i++)
        {
            //Draw debug lines for a clear representation

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

            SDL_FillRect(Dbg,&rleft,SDL_MapRGB(Dbg->format, 255,0,0));
            SDL_FillRect(Dbg,&rright,SDL_MapRGB(Dbg->format, 255,0,0));
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

int NumberOfColumns(HHistogram* histogram,int w)
{
    int c=0;

    int columnNumber = 0;

    int letterNumber = 0;
    int sum = 0;
    float moy = 0;

    while (c < w)
    {
        int taille = columnSize(histogram, c, w,0);
        if (histogram->hist[c] != 0)
        {
            columnNumber++;
            letterNumber++;
            sum += taille;
            moy = (float) sum / (float) letterNumber;
        }

        if (histogram->hist[c] == 0 && c != 0)
        {
            if (taille > (moy) * ((float) 3 / (float) 4))
            {
                columnNumber++;
            }
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

int columnSize(HHistogram* histogram, int start, int w, float pixelLimit)
{
    float startValue = histogram->hist[start];
    int c = 0; 

    if (pixelLimit != 0)
    {
        //offset the limit to accept all cases.
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

                int c = columnSize(hist, columns[i].start, columns[i].end 
                                   , count /((float)(l.end-l.start)));

                //printf("c:%d    start:%f  end:%d    limit:%f\n",c,
                hist->hist[columns[i].start],
                columns[i].end - columns[i].start,
                count /((float)(l.end-l.start)));
            
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

//try to merge two array (not finished)
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

void Stretch_Linear(SDL_Surface* src,SDL_Surface* dest)
{
    double rx,ry;
    //Ratio between the old and the new image.
    rx = dest->w*1.0/src->w;
    ry = dest->h*1.0/src->h;

    for(int i=0;i<dest->w;i++)
    {
        for(int j=0;j<dest->h;j++)
        {
            double valx,valy,fx,fy;
            int minx,miny,maxx,maxy;
            valx = i/rx;
            valy = j/ry;
            minx = (int)valx;
            miny = (int)valy;
            maxx = minx+1;
            if (maxx>=src->w)
                maxx--;
            maxy = miny+1;
            if (maxy>=src->h)
                maxy--;
            fx = valx-minx;  // garde partie flottante
            fy = valy-miny; 

            Uint8 r1,g1,b1;
            Uint32 pixel1 = getpixelval(src,minx,miny);
            SDL_GetRGB(pixel1,dest->format,&r1,&g1,&b1);

            Uint8 r2,g2,b2;
            Uint32 pixel2 = getpixelval(src,maxx,miny);
            SDL_GetRGB(pixel2,dest->format,&r2,&g2,&b2);

            Uint8 r3,g3,b3;
            Uint32 pixel3 = getpixelval(src,minx,maxy);
            SDL_GetRGB(pixel3,dest->format,&r3,&g3,&b3);

            Uint8 r4,g4,b4;
            Uint32 pixel4 = getpixelval(src,maxx,maxy);
            SDL_GetRGB(pixel4,dest->format,&r4,&g4,&b4);
            
            Uint8 rmoy = (Uint8)(r1*(1-fx)*(1-fy) + r2*fx*(1-fy) + r3*(1-fx)*fy + r4*fx*fy);
            Uint8 gmoy = (Uint8)(g1*(1-fx)*(1-fy) + g2*fx*(1-fy) + g3*(1-fx)*fy + g4*fx*fy);
            Uint8 bmoy = (Uint8)(b1*(1-fx)*(1-fy) + b2*fx*(1-fy) + b3*(1-fx)*fy + b4*fx*fy);

            Uint32 newcolor = SDL_MapRGB(dest->format,rmoy,gmoy,bmoy);

            putpixelval(dest,i,j,newcolor);

        }
    }
}