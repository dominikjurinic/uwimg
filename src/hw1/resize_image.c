#include <math.h>
#include "image.h"

float nn_interpolate(image im, float x, float y, int c)
{
  if(x < 0){x = 0;}
  if(y < 0){y = 0;}
	int intX = (int) round(x);
	int intY = (int) round(y);

	 return get_pixel(im, intX, intY, c);
}

image nn_resize(image im, int w, int h)
{
    // TODO Fill in (also fix that first line)
  image newImg = make_image(w,h,im.c);

	float xIncrement = (float) im.w * 1./w;
	float yIncrement = (float) im.h * 1./h;
	float xCoord = 0, yCoord = 0;

	for(int c = 0; c < newImg.c; c++){
		for(int i = 0; i < newImg.h; i++){
			for(int j = 0; j < newImg.w; j++){
				xCoord = xIncrement * (j + .5) - .5;
				yCoord = yIncrement * (i + .5) - .5;
        set_pixel(newImg, j, i, c, nn_interpolate(im, xCoord, yCoord, c));
			}
		}
	}
  return newImg;
}

float bilinear_interpolate(image im, float x, float y, int c)
{
    int LUx = (int) floor(x), LUy = (int) floor(y);   //upper left pixel
    int RUx = (int) ceil(x),  RUy = (int) floor(y);   //upper right pixel
    int LDx = (int) floor(x), LDy = (int) ceil(y);    //lower left pixel
    int RDx = (int) ceil(x),  RDy = (int) ceil(y);    //lower right pixel

    float qc1 = 0;
    float qc2 = 0;

    //interpolate q1 - upper left * lower dist + lower left * upper dist
    qc1 = get_pixel(im, LUx, LUy, c) * (LDy - y) + get_pixel(im, LDx, LDy, c) * (y - LUy);

    //interpolate q2 - upper right * lower dist + lower right * upper dist
    qc2 = get_pixel(im, RUx, RUy, c) * (RDy - y) + get_pixel(im, RDx, RDy, c) * (y - RUy);

    return (qc2 *(x - LUx) + qc1 * (RUx - x));
}

image bilinear_resize(image im, int w, int h)
{
    // TODO
    image newImg = make_image(w, h, im.c);

	  float xIncrement = (float) im.w * 1./ w;
	  float yIncrement = (float) im.h * 1./ h;
	  float xCoord = 0, yCoord = 0;

	  for(int c = 0; c < newImg.c; c++){
		    for(int i = 0; i < newImg.h; i++){
			       for(int j = 0; j < newImg.w; j++){
				          xCoord = xIncrement * (j + .5) - .5;
				          yCoord = yIncrement * (i + .5) - .5;
                  set_pixel(newImg, j, i, c, bilinear_interpolate(im, xCoord, yCoord, c));
			      }
		    }
	  }
	return newImg;
}
