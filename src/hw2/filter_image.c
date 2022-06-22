#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#define TWOPI 6.283185



void l1_normalize(image im)
{
    float sum = 0;
    for(int c = 0; c < im.c; c++){
      for(int i = 0; i < im.h; i++){
        for(int j = 0; j < im.w; j++){
          sum += get_pixel(im, j, i, c);
        }
      }
    }
    for(int c = 0; c < im.c; c++){
      for(int i = 0; i < im.h; i++){
        for(int j = 0; j < im.w; j++){
          set_pixel(im, j, i, c, get_pixel(im, j, i, c)/sum);
        }
      }
    }

}

image make_box_filter(int w)
{
    // TODO
    //square image with one channel with uniform entries that sum to 1.
	  image box_filter = make_image(w, w, 1);
	  for(int adress = 0; adress < box_filter.w * box_filter.h; adress++){
		    box_filter.data[adress] = 1. / (w * w);
	}
    return box_filter;
}

image convolve_image(image im, image filter, int preserve){

    image convolved;
    //image im = make_image(w,h,c);
    //filter and kernel have same number of channels
    //filter corresponding channels with corresponding kernel channels - produce 1 channel picture
    //create 1 channel img
    if(im.c == filter.c && preserve != 1){
      convolved = make_image(im.w, im.h, 1);
      long unsigned int adressCONV;
      for(int c = 0; c < im.c; c++){
        for(int i = 0; i < im.h; i++){
          for(int j = 0; j < im.w; j++){
            adressCONV = i * im.w + j;
            for(int xFilter = 0; xFilter < filter.h; xFilter++){
              for(int yFilter = 0; yFilter < filter.w; yFilter++){
                convolved.data[adressCONV] += get_pixel(filter, yFilter, xFilter, c) * get_pixel(im, j - filter.w/2 + yFilter, i - filter.h/2 + xFilter, c);
              }
            }
          }
        }
      }
      return convolved;
    }
    //filter - produce multi channel pic
    else if(im.c == filter.c && preserve == 1){
      convolved = make_image(im.w, im.h, im.c);
      float individualPixel;
      for(int c = 0; c < im.c; c++){
        for(int i = 0; i < im.h; i++){
          for(int j = 0; j < im.w; j++){
            individualPixel = 0;
            for(int xFilter = 0; xFilter < filter.h; xFilter++){
						    for(int yFilter = 0; yFilter < filter.w; yFilter++){
							  individualPixel += get_pixel(filter, yFilter, xFilter, c) * get_pixel(im, j - filter.w/2 + yFilter, i - filter.h/2 + xFilter, c);
                }
					   }
             set_pixel(convolved, j, i, c, individualPixel);
           }
         }
       }
       return convolved;
     }
     //if filter has only one channel, image multiple channels - 2 cases
     //if preserve = 1 -> preserve image channels
     else if(filter.c == 1 && preserve == 1){
       convolved = make_image(im.w, im.h, im.c);
       float individualPixel;
       for(int c = 0; c < im.c; c++){
         for(int i = 0; i < im.h; i++){
           for(int j = 0; j < im.w; j++){
             individualPixel = 0;
             for(int xFilter = 0; xFilter < filter.h; xFilter++){
 						    for(int yFilter = 0; yFilter < filter.w; yFilter++){
 							  individualPixel += get_pixel(filter, yFilter, xFilter, 0) * get_pixel(im, j - filter.w/2 + yFilter, i - filter.h/2 + xFilter, c);
                 }
 					   }
             set_pixel(convolved, j, i, c, individualPixel);
           }
         }
       }
       return convolved;
     }
     else if(filter.c == 1 && preserve == 0){
       convolved = make_image(im.w, im.h, 1);
       long unsigned int adressCONV = 0;
       for(int c = 0; c < im.c; c++){
         for(int i = 0; i < im.h; i++){
           for(int j = 0; j < im.w; j++){
             adressCONV = i * im.w + j;  //iterates over 1 channel 3 times
             for(int xFilter = 0; xFilter < filter.h; xFilter++){
               for(int yFilter = 0; yFilter < filter.w; yFilter++){
                 convolved.data[adressCONV] += get_pixel(filter, yFilter, xFilter, 0) * get_pixel(im, j - filter.w/2 + yFilter, i - filter.h/2 + xFilter, c);
               }
             }
           }
         }
       }
       return convolved;
     }

    return make_image(1,1,1);
}

image make_highpass_filter()
{
  image highpass = make_image(3,3,1);
  set_pixel(highpass, 0, 0, 0, 0);
	set_pixel(highpass, 1, 0, 0, -1);
	set_pixel(highpass, 2, 0, 0, 0);
	set_pixel(highpass, 0, 1, 0, -1);
	set_pixel(highpass, 1, 1, 0, 4);
	set_pixel(highpass, 2, 1, 0, -1);
	set_pixel(highpass, 0, 2, 0, 0);
	set_pixel(highpass, 1, 2 ,0, -1);
  set_pixel(highpass, 2, 2 ,0, 0);

  return highpass;
}

image make_sharpen_filter()
{
    // TODO
  image sharpen = make_image(3,3,1);
	set_pixel(sharpen, 0, 0, 0, 0);
	set_pixel(sharpen, 1, 0, 0, -1);
	set_pixel(sharpen, 2, 0, 0, 0);
	set_pixel(sharpen, 0, 1, 0, -1);
	set_pixel(sharpen, 1, 1, 0, 5);
	set_pixel(sharpen, 2, 1, 0, -1);
	set_pixel(sharpen, 0, 2, 0, 0);
	set_pixel(sharpen, 1, 2 ,0, -1);
  set_pixel(sharpen, 2, 2 ,0, 0);

	return sharpen;
}

image make_emboss_filter()
{
  image emboss = make_image(3,3,1);
	set_pixel(emboss, 0, 0, 0, -2);
	set_pixel(emboss, 1, 0, 0, -1);
	set_pixel(emboss, 2, 0, 0, 0);
	set_pixel(emboss, 0, 1, 0, -1);
	set_pixel(emboss, 1, 1, 0, 1);
	set_pixel(emboss, 2, 1, 0, 1);
	set_pixel(emboss, 0, 2, 0, 0);
	set_pixel(emboss, 1, 2 ,0, 1);
  set_pixel(emboss, 2, 2 ,0, 2);

	return emboss;

}


image make_gaussian_filter(float sigma)
{

  int kernelSize;
  if((int) round(6 * sigma) % 2 == 0){kernelSize = (int) round(6*sigma) + 1;}
  else                               {kernelSize = (int) round(6 * sigma);}

  float exponent = 0.;
  float gaussianF = 0.;
  image gaussian = make_box_filter(kernelSize);

  for(int i = - kernelSize/2; i <= kernelSize/2; i++){
      for(int j = -kernelSize/2; j <= kernelSize/2; j++){
          exponent =  - (pow(i,2) + pow(j,2))/(2. * pow(sigma,2));
          gaussianF = 1./(TWOPI * pow(sigma,2)) * pow(M_E, exponent);
          set_pixel(gaussian, (i + kernelSize/2) , (j + kernelSize/2), 0, gaussianF);
      }
  }
  l1_normalize(gaussian);
  return gaussian;
}


image add_image(image a, image b)
{
  if(a.h != b.h || a.w != b.w || a.c != b.c){return make_image(1,1,1);} //not same dimension - exit
  image addedImage = make_image(a.w, a.h, a.c);
  float addition = 0;

  for(int c = 0; c < addedImage.c; c++){
    for(int i = 0; i < addedImage.h; i++){
      for(int j = 0; j < addedImage.w; j++){
        addition = get_pixel(a, j, i, c) + get_pixel(b, j, i, c);
        set_pixel(addedImage, j, i, c, addition); //clamping should be already included in set_pixel
      }
    }
  }
  return addedImage;
}

image sub_image(image a, image b)
{
  if(a.h != b.h || a.w != b.w || a.c != b.c){return make_image(1,1,1);} //not same dimension - exit
  image subtractedImage = make_image(a.w, a.h, a.c);
  float subtraction = 0;

  for(int c = 0; c < subtractedImage.c; c++){
    for(int i = 0; i < subtractedImage.h; i++){
      for(int j = 0; j < subtractedImage.w; j++){
        subtraction = get_pixel(a, j, i, c) - get_pixel(b, j, i, c);
        set_pixel(subtractedImage, j, i, c, subtraction); //clamping should be already included in set_pixel
      }
    }
  }
  return subtractedImage;
  }

image make_gx_filter()
{
    // TODO
  image gx_sobel = make_image(3,3,1);
	//void set_pixel(image im, int x, int y, int c, float v)
	set_pixel(gx_sobel, 0, 0, 0, -1);
	set_pixel(gx_sobel, 1, 0, 0,  0);
	set_pixel(gx_sobel, 2, 0, 0,  1);
	set_pixel(gx_sobel, 0, 1, 0, -2);
	set_pixel(gx_sobel, 1, 1, 0,  0);
	set_pixel(gx_sobel, 2, 1, 0,  2);
	set_pixel(gx_sobel, 0, 2, 0, -1);
	set_pixel(gx_sobel, 1, 2, 0,  0);
	set_pixel(gx_sobel, 2, 2 , 0, 1);

	return gx_sobel;
}

image make_gy_filter()
{
    // TODO
  image gy_sobel = make_image(3,3,1);
	//void set_pixel(image im, int x, int y, int c, float v)
	set_pixel(gy_sobel, 0, 0, 0, -1);
	set_pixel(gy_sobel, 1, 0, 0, -2);
	set_pixel(gy_sobel, 2, 0, 0,  -1);
	set_pixel(gy_sobel, 0, 1, 0,  0);
	set_pixel(gy_sobel, 1, 1, 0,  0);
	set_pixel(gy_sobel, 2, 1, 0,  0);
	set_pixel(gy_sobel, 0, 2, 0, 1);
	set_pixel(gy_sobel, 1, 2, 0,  2);
	set_pixel(gy_sobel, 2, 2, 0, 1);

	return gy_sobel;
}

void feature_normalize(image im)
{
    // TODO

	float minR = get_pixel(im, 0, 0, 0), maxR = get_pixel(im, 0, 0, 0);
	float minG = get_pixel(im, 0, 0, 1), maxG = get_pixel(im, 0, 0, 1);
	float minB = get_pixel(im, 0, 0, 2), maxB = get_pixel(im, 0, 0, 2);


	for(int c = 0; c < im.c; c++){
		for(int i = 0; i < im.h; i++){
			for(int j = 0; j < im.w; j++){
				if	   (get_pixel(im, j, i ,c) < minR && c == 0){minR = get_pixel(im, j, i, c);}
				else if(get_pixel(im, j, i, c) > maxR && c == 0){maxR = get_pixel(im, j, i, c);}
				else if(get_pixel(im, j, i, c) < minG && c == 1){minG = get_pixel(im, j, i, c);}
				else if(get_pixel(im, j, i, c) > maxG && c == 1){maxG = get_pixel(im, j, i, c);}
				else if(get_pixel(im, j, i, c) < minB && c == 2){minB = get_pixel(im, j, i, c);}
				else if(get_pixel(im, j, i, c) > maxB && c == 2){maxB = get_pixel(im, j, i, c);}
			}
		}
	}

	float rangeR = maxR - minR;
	float rangeG = maxG - minG;
	float rangeB = maxB - minB;

	float normalisedValue = 0;

	for(int c = 0; c < im.c; c++){
		for(int i = 0; i < im.h; i++){
			for(int j = 0; j < im.w; j++){
				if(c == 0){	//R channel
					if(rangeR == 0){set_pixel(im, j, i, c, 0.);}
					else{
						normalisedValue = (get_pixel(im, j, i, c) - minR)/(rangeR);
						set_pixel(im, j, i, c, normalisedValue);
					}
				}
				else if(c == 1){	//G channel
					if(rangeG == 0){set_pixel(im, j, i, c, 0.);}
					else{
						normalisedValue = (get_pixel(im, j, i, c) - minG)/(rangeG);
						set_pixel(im, j, i, c, normalisedValue);
					}
				}
				else if(c == 2){	//B channel
					if(rangeB == 0){set_pixel(im, j, i, c, 0.);}
					else{
						normalisedValue = (get_pixel(im, j, i, c) - minB)/(rangeB);
						set_pixel(im, j, i, c, normalisedValue);
					}
				}
			}
		}
	}
}

image *sobel_image(image im)
{
    // TODO
  image g_x = convolve_image(im, make_gx_filter(), 0);  //1 channel image
	image g_y = convolve_image(im, make_gy_filter(), 0);  //1 channel image

	float xPixel = 0, yPixel = 0;
  image *ret = calloc(2, sizeof(image));
  ret[0] = make_image(im.w, im.h, 1);
  ret[1] = make_image(im.w, im.h, 1);
	for(int i = 0; i < im.h; i++){
		for(int j = 0; j < im.w; j++){
			xPixel = get_pixel(g_x, j, i, 0);
			yPixel = get_pixel(g_y, j, i, 0);
      ret[0].data[i * im.w + j] = sqrt(pow(xPixel,2) + pow(yPixel,2));
      ret[1].data[i * im.w + j] = atan2(yPixel, xPixel);
		}
	}
    //return calloc(2, sizeof(image));
    return ret;
}

image colorize_sobel(image im)
{
    // TODO
    return make_image(1,1,1);
}
