#include <math.h>
#include<stdio.h>
#include "image.h"

float nn_interpolate(image im, float x, float y, int c)
{
    
    int fx,fy;

    if(x - (int)x <= .5)
        fx = (int)x;
    else 
        fx = (int)x + 1;

    if(y - (int)y <= .5)
        fy = (int)y;
    else 
        fy = (int)y + 1;

    return get_pixel(im, fx, fy, c);
}

image nn_resize(image im, int w, int h)
{
    image n_image = make_image(w,h,im.c);

    for(int c=0; c<im.c; c++) { 
        for(int i=0;i<h;i++) {
            for(int j = 0; j < w;j++) {
                set_pixel(n_image, j, i ,c, nn_interpolate(im, j*im.w/w, i*im.h/h, c));
            }
        }
    }

    return n_image;
}

float bilinear_interpolate(image im, float x, float y, int c)
{
    float tl, tr, bl, br;
    
    tl = get_pixel(im, (int)x, (int)y, c);
    bl = get_pixel(im, (int)x, (int)y+1, c);
    tr = get_pixel(im, (int)x+1, (int)y, c);
    br = get_pixel(im, (int)x+1, (int)y+1, c);

    float valt,valf, valb;

    float d1, d2, d3, d4;

    float ix, iy;

    ix = (int) x;
    iy = (int) y;

    d1 = x-ix;
    d2 = ix+ 1 - x;
    d3= y - iy;
    d4= iy+ 1 - y;

    valt = d1*tr + d2*tl; 
    valb = d2*bl + d1*br; 

    valf = d2*tl*d4 + d1*tr*d4 +  d2*bl*d3 + d1*br*d3;

    //printf("x: %f, y: %f, d1: %f, d2: %f, d3: %f, d4: %f, tl: %f, tr: %f, bl: %f, br: %f \n", x, y, d1, d2, d3, d4, tl, tr, bl, br);

    return valf;
}

image bilinear_resize(image im, int w, int h)
{
    image n_image = make_image(w,h,im.c);

    float x, y;
    for(int c=0; c<im.c; c++) { 
        for(int i=0;i<h;i++) {
            for(int j = 0; j < w;j++) {
                x = j*im.w;
                x/=w;

                y= i*im.h;
                y/=h;
                set_pixel(n_image, j, i ,c, bilinear_interpolate(im, x, y, c));
            }
        }
    }

    return n_image;
}

