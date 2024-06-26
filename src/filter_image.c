#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#define TWOPI 6.2831853

void l1_normalize(image im)
{
    float sum=0;
    for(int c=0;c<im.c;c++){
        for(int j = 0; j <im.h;j++) {
            for(int i=0; i<im.w; i++) {
                sum+=get_pixel(im, i, j, c);
            }
        }
    }

    for(int c=0;c<im.c;c++){
        for(int j = 0; j <im.h;j++) {
            for(int i=0; i<im.w; i++) {
                set_pixel(im, i, j, c, get_pixel(im, i, j, c)/sum);
            }
        }
    }
}

image make_box_filter(int w)
{
    image im = make_image(w,w,1);

    for(int c=0;c<im.c;c++){
        for(int j = 0; j <im.h;j++) {
            for(int i=0; i<im.w; i++) {
                set_pixel(im, i, j, c, w);
            }
        }
    }

    l1_normalize(im);
    
    return im;
}

image convolve_image(image im, image filter, int preserve)
{
    image ret;
    float csum;
    if(preserve == 1)
        ret = make_image(im.w, im.h, im.c);
    else 
        ret = make_image(im.w, im.h, 1);

    
    for(int j=-1*filter.h/2;j<im.h + filter.h/2;j++) {
        for(int i= -1*filter.w/2;i<im.w + filter.w/2; i++) { //For image
            csum=0;

            if(preserve == 1) {
                if(im.c == filter.c) {
                    for(int c=0; c<im.c; c++) {
                        for(int jfil=0; jfil<filter.h; jfil++) {
                            for(int ifil=0; ifil<filter.w; ifil++ ){
                                csum+=get_pixel(filter, ifil, jfil, c) * get_pixel(im, i + ifil, j + jfil, c);
                            }
                        }
                        set_pixel(ret, i, j, c, csum);
                        csum=0;
                    }
                } else {
                    for(int c=0; c<im.c; c++) {
                        for(int jfil=0; jfil<filter.h; jfil++) {
                            for(int ifil=0; ifil<filter.w; ifil++ ){
                                csum+=get_pixel(filter, ifil, jfil, 0) * get_pixel(im, i+ifil, j+jfil, c);
                            }
                        }
                        //printf("Oui");
                        set_pixel(ret, i, j, c, csum);
                        csum=0;
                    }
                }
            } else {
                if(im.c == filter.c) {
                    for(int c=0; c<im.c; c++) {
                        for(int jfil=0; jfil<filter.h; jfil++) {
                            for(int ifil=0; ifil<filter.w; ifil++ ){
                                csum+=get_pixel(filter, ifil, jfil, c) * get_pixel(im, i+ifil, j+jfil, c);
                            }
                        }   
                    }
                    set_pixel(ret, i, j, 0, csum);
                    csum=0;
                } else {
                    for(int c=0; c<im.c; c++) {
                        for(int jfil=0; jfil<filter.h; jfil++) {
                            for(int ifil=0; ifil<filter.w; ifil++ ){
                                csum+=get_pixel(filter, ifil, jfil, 0) * get_pixel(im, i+ifil, j+jfil, c);
                            }
                        }   
                    }
                    set_pixel(ret, i, j, 0, csum);
                    csum=0;
                }
            }
        }
    }
    
    return ret;
}

image make_highpass_filter()
{
    image ret = make_box_filter(3);

    set_pixel(ret, 1, 0, 0, -1);
    set_pixel(ret, 1, 2, 0, -1);
    set_pixel(ret, 0, 1, 0, -1);
    set_pixel(ret, 2, 1, 0, -1);
    set_pixel(ret, 1, 1, 0, 4);

    l1_normalize(ret);
    return ret; 
}

image make_sharpen_filter()
{
    image ret = make_box_filter(3);

    set_pixel(ret, 1, 0, 0, -1);
    set_pixel(ret, 1, 2, 0, -1);
    set_pixel(ret, 0, 1, 0, -1);
    set_pixel(ret, 2, 1, 0, -1);
    set_pixel(ret, 1, 1, 0, 5);

    l1_normalize(ret);
    return ret; 
}

image make_emboss_filter()
{
    image ret = make_box_filter(3);

    set_pixel(ret, 0, 0, 0, -2);
    set_pixel(ret, 1, 0, 0, -1);
    set_pixel(ret, 1, 2, 0, 1);
    set_pixel(ret, 0, 1, 0, -1);
    set_pixel(ret, 2, 1, 0, 1);
    set_pixel(ret, 1, 1, 0, 1);
    set_pixel(ret, 2, 2, 0, 2);

    l1_normalize(ret);
    return ret; 
}

// Question 2.2.1: Which of these filters should we use preserve when we run our convolution and which ones should we not? Why?
// Answer: TODO

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: TODO

image make_gaussian_filter(float sigma)
{
    image ret;
    
    if( ( (6*(int)sigma) % 2 ) == 0 ) 
        ret = make_box_filter(6*sigma + 1);
    else 
        ret = make_box_filter(6*sigma);

float val;
int jfal, ifal;

 jfal=0;
    // for(int j=-1*ret.h/2; j<=ret.h/2; j++,jfal++) {
    //     ifal=0;
    //     for(int i=-1*ret.w/2; i<=ret.w/2; i++, ifal++) {
    //         printf("%d,%d(%d, %d)|\t",i,j,ifal,jfal);
    //     }
    //     printf("\n");
    // }
float exparg;
    jfal=0;
    for(int j=-1*ret.h/2; j<=ret.h/2; j++,jfal++) {
        ifal=0;
        for(int i=-1*ret.w/2; i<=ret.w/2; i++, ifal++) {
            exparg = -(i*i + j*j) / (2*sigma*sigma);
            val =   expf(exparg)/(2*M_PI*sigma*sigma); 
            set_pixel(ret, ifal, jfal, 0, val);
            //printf("%f |\t", val);
        }
        //printf("\n");
    }

    l1_normalize(ret);

    //printf("\n");printf("\n");

    // for(int j=0; j<ret.h; j++) {
    //     for(int i=0; i<ret.w; i++) {
    //         printf("%f|\t",get_pixel(ret, i, j, 0));
    //     }
    //     printf("\n");
    // }

    return ret;
}

image add_image(image a, image b)
{
    // TODO

    image ret = make_image(a.w, a.h, a.c);

    for(int c=0; c<a.c; c++) {
        for(int j=0; j<a.h;j++) {
            for(int i=0; i<a.w;  i++) {
                set_pixel(ret, i, j, c, get_pixel(a, i, j, c)+get_pixel(b, i, j, c));
            }
        }
    }

    return ret;
}

image sub_image(image a, image b)
{
   image ret = make_image(a.w, a.h, a.c);

    for(int c=0; c<a.c; c++) {
        for(int j=0; j<a.h;j++) {
            for(int i=0; i<a.w;  i++) {
                set_pixel(ret, i, j, c, get_pixel(a, i, j, c) - get_pixel(b, i, j, c));
            }
        }
    }

    return ret;
}

image make_gx_filter()
{
    image ret = make_box_filter(3);

    set_pixel(ret,0,0,0, -1);
    set_pixel(ret,1,0,0, 0);
    set_pixel(ret,2,0,0, 1);

    set_pixel(ret,0,1,0, -2);
    set_pixel(ret,1,1,0, 0);
    set_pixel(ret,2,1,0, 2);

    set_pixel(ret,0,2,0, -1);
    set_pixel(ret,1,2,0, 0);
    set_pixel(ret,2,2,0, 1);

    return ret;
}

image make_gy_filter()
{
    image ret = make_box_filter(3);

    set_pixel(ret,0,0,0, -1);
    set_pixel(ret,1,0,0, -2);
    set_pixel(ret,2,0,0, -1);

    set_pixel(ret,0,1,0, 0);
    set_pixel(ret,1,1,0, 0);
    set_pixel(ret,2,1,0, 0);

    set_pixel(ret,0,2,0, 1);
    set_pixel(ret,1,2,0, 2);
    set_pixel(ret,2,2,0, 1);

    return ret;
}

void feature_normalize(image im)
{
    float minval=300, maxval=-300;

    for(int c=0; c<im.c;c++) {
        for(int j=0; j<im.h; j++) {
            for(int i=0; i<im.w; i++) {
                if(get_pixel(im, i,j,c) < minval)
                    minval = get_pixel(im, i,j,c);
                
                if(get_pixel(im, i,j,c) > maxval)
                    maxval = get_pixel(im, i,j,c);
            }
        }
    }

    float range = maxval-minval;

    for(int c=0; c<im.c;c++) {
        for(int j=0; j<im.h; j++) {
            for(int i=0; i<im.w; i++) {
                set_pixel(im, i,j, c, (get_pixel(im, i, j, c) - minval)/range);
            }
        }
    }
}

image *sobel_image(image im)
{
    image* ret = calloc(2, sizeof(image));

    image sobelh, sobelv;

    sobelh= make_gx_filter();
    sobelv= make_gx_filter();

    image inter1 = convolve_image(im, sobelh, 0);
    image inter2 = convolve_image(im, sobelv, 0);

    for(int j=0;j<im.h;j++) {
        for(int i=0;i<im.w; i++) {
            set_pixel(ret[0], i, j, 0, sqrtf(powf(get_pixel(inter1, i,j,0),2) + powf(get_pixel(inter2, i,j,0),2)));
        }
    }

    for(int j=0;j<im.h;j++) {
        for(int i=0;i<im.w; i++) {
            set_pixel(ret[1], i, j, 0, atan2f(get_pixel(inter2, i,j,0), get_pixel(inter1, i,j,0)));
        }
    }

    return ret;
}

image colorize_sobel(image im)
{
    // TODO
    return make_image(1,1,1);
}
