#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <math.h>
#include "image.h"

float get_pixel(image im, int x, int y, int c)
{
    if(x < 0){x = 0;}
    if(x >= im.w){x = im.w - 1;}
    if(y < 0){y = 0;}
    if(y >= im.h){y = im.h - 1;}
    unsigned int adress = c * (im.h * im.w) + y * im.w + x;
    return im.data[adress];
}

void set_pixel(image im, int x, int y, int c, float v)
{
    if(x < 0 || x >= im.w || y < 0 || y >= im.h){return;}
    else{unsigned int adress = c * (im.h * im.w) + y * im.w + x;
		im.data[adress] = v;
	}
	return;
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    // TODO Fill this in
    for(int i = 0; i < im.w*im.h*im.c; i++){
      copy.data[i] = im.data[i];
    }
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    long unsigned int adressGray = 0, adressR = 0, adressG = 0, adressB = 0;
    // TODO Fill this in
	  for(int i = 0; i < im.h; i++){
		    for(int j = 0; j < im.w; j++){
			       adressGray = i * im.w + j;
			       adressR =                     i * im.w + j;
			       adressG =     (im.h * im.w) + i * im.w + j;
			       adressB = 2 * (im.h * im.w) + i * im.w + j;
			       gray.data[adressGray] = .299 * im.data[adressR] + .587 * im.data[adressG] + .114 * im.data[adressB];
		    }
	  }
    return gray;
}

void shift_image(image im, int c, float v)
{
    // TODO Fill this in
    long unsigned int adress = 0;
    for(int i = 0; i < im.h; i++){
	     for(int j = 0; j < im.w; j++){
		       adress = c * (im.h * im.w) + i * im.w + j;
		       im.data[adress] = im.data[adress] + v;
		}
	}
}

void clamp_image(image im)
{
    // TODO Fill this in
    long unsigned int adress = 0;
	  for(int i = 0; i < im.c; i++){
      for(int j = 0; j < im.h; j++){
        for(int k = 0; k < im.w; k++){
          adress = i * (im.h * im.w) + j * im.w + k;
          if     (im.data[adress] < 0){im.data[adress] = 0.;}
          else if(im.data[adress] > 1){im.data[adress] = 1.;}
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

    // TODO Fill this in
    long unsigned int adressR = 0, adressG = 0, adressB = 0;
    float V = 0, m = 0, C = 0, S, Hdot = 0, H = 0;
      for(int i = 0; i < im.h; i++){
	       for(int j = 0; j < im.w; j++){
		         adressR =                     i * im.w + j;
		         adressG =     (im.h * im.w) + i * im.w + j;
		         adressB = 2 * (im.h * im.w) + i * im.w + j;
		         V = three_way_max(im.data[adressR], im.data[adressG], im.data[adressB]);
 		         m = three_way_min(im.data[adressR], im.data[adressG], im.data[adressB]);
		         C = V - m;

    			   S = C / three_way_max(im.data[adressR], im.data[adressG], im.data[adressB]);

    			   if(C == 0){H = 0.;}
    		 	   else if(V == im.data[adressR]){Hdot = (im.data[adressG] - im.data[adressB])/C;}
    			   else if(V == im.data[adressG]){Hdot = (im.data[adressB] - im.data[adressR])/C + (float) + 2.;}
    			   else if(V == im.data[adressB]){Hdot = (im.data[adressR] - im.data[adressG])/C + (float) + 4.;}
			          if(Hdot < 0){H = Hdot/6. + 1.;}
			          else{H = Hdot/6.;}
            if(im.data[adressR] == im.data[adressG] && im.data[adressR] == im.data[adressB] && im.data[adressG] == im.data[adressB] ){
              im.data[adressR] = 0;
        			im.data[adressG] = 0;
            }
              else{
			             im.data[adressR] = H;
			             im.data[adressG] = S;
			             im.data[adressB] = V;
                       }
			}
		}

}

void hsv_to_rgb(image im)
{
  long unsigned int adressH = 0, adressS = 0, adressV = 0;
  float C, X, m, Hdot, Rdot, Gdot, Bdot;
  for(int i = 0; i < im.h; i++){
    for(int j = 0; j < im.w; j++){


      C = get_pixel(im, j, i, 1) * get_pixel(im, j, i, 2);
      Hdot = get_pixel(im, j, i,0) * (float) 60;
      X = (float) C * (1 - abs( fmod(Hdot/60, 2) - 1.));
      m = get_pixel(im, j, i, 2) - C;

      if(im.data[adressH] >= 0 && im.data[adressH] < 60){
        Rdot = C; Gdot = X; Bdot = 0;}
      else if(im.data[adressH] >= 60 && im.data[adressH] < 120){
        Rdot = X; Gdot = C; Bdot = 0;}
      else if(im.data[adressH] >= 120 && im.data[adressH] < 180){
        Rdot = 0; Gdot = C; Bdot = X;}
      else if(im.data[adressH] >= 180 && im.data[adressH] < 240){
        Rdot = 0; Gdot = X; Bdot = C;}
      else if(im.data[adressH] >= 240 && im.data[adressH] < 300){
        Rdot = X; Gdot = 0; Bdot = C;}
      else if(im.data[adressH] >= 300 && im.data[adressH] < 360){
        Rdot = C; Gdot = 0; Bdot = X;}

      set_pixel(im, j, i, 0, Rdot + m);
      set_pixel(im, j, i, 1, Gdot + m);
      set_pixel(im, j, i, 2, Bdot + m);

    }
  }
}
