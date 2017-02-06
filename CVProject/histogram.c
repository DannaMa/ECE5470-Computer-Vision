#include "VisXV4.h"           /* VisionX structure include file    */
#include "Vutil.h"            /* VisionX utility header files      */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

VXparam_t par[] =             /* command line structure            */
{ /* prefix, value,   description                         */   
{    "if=",    0,   " input file  vtemp: local max filter "},
{    "of=",    0,   " output file "},
{     0,       0,    0}
};

#define  IVAL   par[0].val
#define  OVAL   par[1].val

Vfstruct (im);                      /* i/o image structure          */
Vfstruct (tm);                      /* temp image structure         */


int i,y,x;                        /* index counters */    
int hist[256];

main(argc, argv)
int argc;
char *argv[];
{
  VXparse(&argc, &argv, par);       /* parse the command line       */
  Vfread(&im, IVAL);                /* read image file              */
  Vfembed(&tm, &im, 1,1,1,1);       /* image structure with border  */
  if ( im.type != VX_PBYTE ) {      /* check image format           */
     fprintf(stderr, "vtemp: no byte image data in input file\n");
     exit(-1);
  }

/* Initialize the histogram array. */

   for (i = 0; i < 256; i++)
	hist[i] = 0;

   for (y = im.ylo ; y <= im.yhi ; y++) {
	for (x = im.xlo; x <= im.xhi; x++)  {
        hist[tm.u[y][x]]++;
     	}
   }

// Histogram stats min max avg
     
    int hsum=0;
    float avg;
    int hmax=-1,hmin=257;
    for(i=0;i<256;i++)
      {
         if (hist[i]>hmax)
         hmax=hist[i];
         if (hist[i]<hmin)
         hmin=hist[i];
      }
 
    for(i=0;i<256;i++)
         hsum+=hist[i]*i;
         avg=(float)hsum/(im.xhi*im.yhi);

//Dispersion of data for original image (Standar Deviation)
    float cSum=0;
    float cTable[256];
    float avgDev=0;
    for(i=0;i<256;i++)
    {
            cTable[i]=(abs((i-(int)avg)))*(abs((i-(int)avg)));
            cTable[i]=hist[i]*cTable[i];
            cSum+=cTable[i];
    }        
    cSum=cSum/im.xhi*im.yhi;
    avgDev=sqrt(cSum);
         
         
    printf("\nOriginal Histogram statistics");
    printf("\nSum: %d \n Max: %d\n Min: %d \n Mean: %f \n Std: %f",hsum,hmax,hmin,avg,avgDev);
  
//Probability table
 
float prt[256];
for(i=0;i<256;i++)
prt[i]=(float)hist[i]/(im.xhi*im.yhi);     

//Cumulative Distribution Function
float cdfmax=-1,cdfmin=257;
float cdf[256];
cdf[0]=prt[0];
for(i=1;i<256;i++)
{
        cdf[i]=prt[i]+cdf[i-1];
        if(cdf[i]>cdfmax)
        cdfmax=cdf[i];
        if(cdf[i]<cdfmin)
        cdfmin=cdf[i];
}
     
//  Equalized Histogram
int eHist[256];
for(y=0;y<256;y++)
for(x=0;x<256;x++)
eHist[im.u[y][x]]++;
 

//Equalized Histogram stats
    int ehsum=0;
    float eavg;
    int ehmax=-1,ehmin=257;
    for(i=0;i<256;i++)
      {
         if (eHist[i]>ehmax)
         ehmax=eHist[i];
         if (eHist[i]<ehmin)
         ehmin=eHist[i];
      }
 
    for(i=0;i<256;i++)
            ehsum+=eHist[i]*i;
 
        eavg=(float)ehsum/(im.xhi*im.yhi);
// Dispersion of data for Equalised Histogram  
    float ecSum=0;
    float ecTable[256];
    float eavgDev=0;
    for(i=0;i<256;i++)
    {
            ecTable[i]=(abs((i-(int)eavg)))*(abs((i-(int)eavg)));
            ecTable[i]=eHist[i]*ecTable[i];
            ecSum+=ecTable[i];
    }        
    ecSum=ecSum/(im.xhi*im.yhi);
     
     
    eavgDev=sqrt(ecSum);
    printf("\n\n Equalised Histogram statistics");
    printf("\n Sum: %d \n Max: %d \n Min: %d \n Mean: %f \n Std: %f",ehsum,ehmax,ehmin,eavg,eavgDev);


//  Final Image
for (y=0;y<im.yhi;y++)
{
    for(x=0;x<im.xhi;x++)
    {
    im.u[y][x]=cdf[tm.u[y][x]]*255;
    }
}

   Vfwrite(&im, OVAL);             /* write image file                */
   exit(0);


}


