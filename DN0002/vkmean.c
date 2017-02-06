#include "VisXV4.h"          /* VisionX structure include file       */
#include "Vutil.h"           /* VisionX utility header files         */
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

VXparam_t par[] =            /* command line structure               */
{ /* prefix, value,   description  */
{  "if=",   0,   " input file, vtpeak: threshold between hgram peaks"},
{  "of=",   0,   " output file "},
{   0,      0,   0} /* list termination */
};
#define  IVAL   par[0].val
#define  OVAL   par[1].val
Vfstruct (im);                 /* input image structure          */	   


#define classNum 10
int k;/* number of clusters*/
int x,y;/*corrdinate index*/
int minDistance,Distance;//corrdinate index for previous centroid xy coordinate
int label[1000][1500];
double centroidPoints[classNum];
double prev_error,error;
double t=2.0; /*error tolerance*/
void setlabel(int, int);


main(argc, argv)
int argc;
char *argv[];
{
  
    VXparse(&argc, &argv, par);       /* parse the command line       */
    Vfread(&im, IVAL);                /* read image file              */

  if ( im.type != VX_PBYTE ) {      /* check image format           */
     fprintf(stderr, "vtemp: no byte image data in input file\n");
     exit(-1);
  }

//Initialize classPoints
for (k=0; k < classNum; k++){
 centroidPoints[k]=80+175/classNum*k;
}


do{
/* save error from last step */
error =0.0;
// Initialize Dataset
   for (y = im.ylo ; y <= im.yhi ; y++) {
	for (x = im.xlo; x <= im.xhi; x++)  {
		minDistance = 255;	
		for (k=0; k < classNum; k++){
		Distance = abs(im.u[y][x]-centroidPoints[k]);
			if (Distance < minDistance) {
			minDistance = Distance;
			label[y][x]=k;
			}
		}
  	}
   }

/*update the centroid intensity*/
for (k=0; k <classNum; k++){
  int count =0;
  int intensity = 0;
	for (y = im.ylo ; y <= im.yhi ; y++) {
	    for (x = im.xlo; x <= im.xhi; x++)  {
		if (k==label[y][x]){
		intensity += im.u[y][x];
		count++;
		}		
	    }
	}	
   error += abs(centroidPoints[k]-intensity/count);
   centroidPoints[k] = intensity/count;
   }
}while(error > t);


/*output the clustered result*/
for (y = im.ylo ; y <= im.yhi ; y++) {
   for (x = im.xlo; x <= im.xhi; x++)  {
	if(im.u[y][x]!=0) im.u[y][x]= 255/classNum*label[y][x];
   }
}

Vfwrite(&im, OVAL);             /* write image file                */
exit(0);
}


