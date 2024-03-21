#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"
#define MAX(X,Y) ((X)<(Y)?(Y):(X))
#define MIN(X,Y) ((X)<(Y)?(X):(Y))
float get_pixel(image im, int x, int y, int c)
{

    //Using clamp strategy
    x = MAX(0, x);
    y = MAX(0, y);
    c = MAX(0, c);

    x = MIN(im.w - 1, x);  // -1 because we are using 0 indexing, i.e., for an image of 256 x 256 x 3, we can only access 0-255 row and column, and 0-2 channels.
    y = MIN(im.h - 1, y);
    c = MIN(im.c - 1, c);

    if(x<0 || y<0 || c<0 || x>= im.w || y >=im.h || c>=im.c)
        return 0;
    /*
        Image has a structure like:

        typedef struct{
            int h, w, c;
            float *data;
        } image;
    */

   //x, y, c -> For column x, row y and channel c.

    return im.data[im.h*im.w*c + y*im.w + x];

    return 0;
}

void set_pixel(image im, int x, int y, int c, float v)
{
    //Pass without doing anything in case out of bounds coordinates are provided.

    if(x < 0 || y<0 || c < 0 || x >= im.w || y >= im.h || c >= im.c)
        return;
    
    im.data[im.h*im.w*c + y*im.w + x] = v;
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    memcpy(copy.data, im.data, sizeof(float) * copy.h*copy.w*copy.c); //Memcpy copies by byte to a memory location, it is variable-agnostic, so we need to provide number of bytes to copy.
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    
    for(int i=0;i < im.h; i++) {
        for(int j=0; j < im.w;j++) {
            gray.data[i*im.w + j] = 0.299 * im.data[i*im.w + j] + 0.587 * im.data[im.w*im.h + i*im.w + j] + 0.114 * im.data[2*im.w*im.h + i*im.w + j];
        }
    }

    return gray;
}

void shift_image(image im, int c, float v)
{
    for(int i=0; i<im.h; i++){
        for(int j=0; j<im.w; j++) {
            im.data[c*im.h*im.w + i*im.w +j] = im.data[c*im.h*im.w + i*im.w +j] + v; 
        }
    }
}

void clamp_image(image im)
{
    for(int z=0;z<im.c;z++) {
        for(int i=0;i<im.h;i++){
            for(int j=0;j<im.w;j++) {
                im.data[z*im.h*im.w + i*im.w+ j] = MIN(1, im.data[z*im.h*im.w + i*im.w+ j]);
                im.data[z*im.h*im.w + i*im.w+ j] = MAX(0, im.data[z*im.h*im.w + i*im.w+ j]);  
            }
        }
    }
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    //R replaced by H
    //G replaced by S
    //B replaced by V

    float r,g,b;

    float Val, minVal, Sat, C, Hdash, H;

    for(int i=0;i<im.w;i++) {
        for(int j=0; j<im.h;j++) {

            r=get_pixel(im, i, j, 0);
            g=get_pixel(im, i, j, 1);
            b=get_pixel(im, i, j, 2);

            //Value is the biggest of RGB
            Val = three_way_max(r, g, b);
            
            //For Saturation:
            minVal= three_way_min(r, g ,b);
            C = Val - minVal;

            if(Val != 0.0)
                Sat = C/Val;
            else
                Sat = 0;

            if(C != 0.0) {
                if(Val == r) 
                    Hdash = (g-b)/C;
                else if (Val==g)  
                    Hdash = ((b-r)/C) + 2;
                else
                    Hdash = ((r-g)/C) + 4;

                if(Hdash<0)
                    H = Hdash/6 + 1;
                else 
                    H = Hdash/6;
            } else {
                H = 0;
            }

            set_pixel(im, i, j, 0, H);
            set_pixel(im, i, j, 1, Sat);
            set_pixel(im, i, j, 2, Val); 
             
        }
    }
}

void hsv_to_rgb(image im)
{
    float H,S,V, R,G,B, C, minVal, maxVal;

    for(int j=0;j<im.h;j++){
        for(int i=0;i<im.w;i++) {
            H=get_pixel(im, i, j, 0);
            S=get_pixel(im, i, j, 1);
            V=get_pixel(im, i, j, 2);
            
            C = V*S;
            minVal = V - C;
            maxVal = V;
            
            //Check where H lies in the curve
            H = H*6;

            if(H>=0 && H<=1) {
                //R is max, G>B
                R = maxVal;
                B = minVal;
                G = C*H + B; 
            } else if(H>1 && H<=2) {
                //G is max, R>B
                G = maxVal;
                B = minVal;
                R = B - C*(H-2); 
            } else if(H>2 && H<=3) {
                 //G is max, B>R
                G = maxVal;
                R = minVal;
                B = C*(H-2) + R; 
            } else if(H>3 && H<=4) {
                //B is max, G>R
                B = maxVal;
                R = minVal;
                G = R - C*(H-4); 
            } else if(H>4 && H<=5) {
                //B is max, R>G
                B = maxVal;
                G = minVal;
                R = C*(H-4) + G; 
            } else if(H>5) {
               //R is max, B>G
                H = H - 6;
                R = maxVal;
                G = minVal;
                B = G - C*H; 
            }

            set_pixel(im, i, j, 0, R);
            set_pixel(im, i, j, 1, G);
            set_pixel(im, i, j, 2, B);
        }
    }
}
