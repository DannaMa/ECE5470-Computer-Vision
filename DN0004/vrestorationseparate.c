/*******************************************************************/
/* vtemp      Compute local max operation on a single byte image   */
/*******************************************************************/

#include "VisXV4.h"           /* VisionX structure include file    */
#include "Vutil.h"            /* VisionX utility header files      */
#include <stdio.h>
#include <stdlib.h>

VXparam_t par[] =             /* command line structure            */
{ /* prefix, value,   description                         */   
{    "if=",    0,   " input file  vtemp: local max filter "},
{    "of=",    0,   " output file "},
{     "r=",    0,   " separate pixel value"},
{     0,       0,   0}  /* list termination */
};
#define  IVAL   par[0].val
#define  OVAL   par[1].val
#define  RVAL   par[2].val

void setzero(int, int);
Vfstruct (im);                      /* i/o image structure          */
Vfstruct (tm);                      /* temp image structure         */

main(argc, argv)
int argc;
char *argv[];
{
int        y,x;                     /* index counters               */
int        r;                       /* region pixel range           */

  VXparse(&argc, &argv, par);       /* parse the command line       */

  r = 0;                    /* default dist */
  if (RVAL) r = atoi(RVAL);  /* if d= was specified, get value */
  if (r < 0 || r > 255) {
	fprintf(stderr, "r= must be between 0 and 255\nUsing r=10\n");
        r = 0;
    }

  Vfread(&im, IVAL);                /* read image file              */
  Vfembed(&tm, &im, 1,1,1,1);       /* image structure with border  */
  if ( im.type != VX_PBYTE ) {      /* check image format           */
     fprintf(stderr, "vtemp: no byte image data in input file\n");
     exit(-1);
  }
  for (y = im.ylo ; y <= im.yhi ; y++) {
     for (x = im.xlo; x <= im.xhi; x++)  {
	im.u[y][x] = 0;
     }
   }

 for (y = tm.ylo; y <= tm.yhi ; y++) {    
          tm.u[y][tm.xlo]=255;
	  tm.u[y][tm.xhi]=255;}
 for (x = tm.xlo; x <= tm.xhi; x++){
	  tm.u[tm.ylo][x]=255;
	  tm.u[tm.yhi][x]=255;	
 }

  for (y = im.ylo ; y <= im.yhi ; y++) {
     for (x = im.xlo; x <= im.xhi; x++)  {
	if (tm.u[y][x] == 168 && im.u[y][x] == 0) {
	    if(tm.u[y+1][x] ==0 || tm.u[y-1][x] ==0 || tm.u[y][x+1] ==0 || tm.u[y][x-1] ==0) {
		setzero(x,y);}		
	    im.u[y][x] = 255;
	}
     }
   }
   Vfwrite(&im, OVAL);             /* write image file                */
   exit(0);
}

/* function to set the label */
void setzero(int x, int y){
//printf("imadsfdasfdsaf");
//printf("im=%d, x=%d, y=%d\n",im.u[y][x],x,y);
	im.u[y][x] = 0;
	tm.u[y][x] = 255;
//printf("ima=%d, xa=%d, ya=%d\n",im.u[y][x],x,y);
	if (tm.u[y+1][x] ==168 ) {
		setzero(x, y+1);
	}
	if (tm.u[y-1][x] ==168 ) {
		setzero(x, y-1);
	}
	if (tm.u[y][x-1] ==168 ) {
		setzero(x-1, y);
	}
	if (tm.u[y][x+1] ==168 ) {
		setzero(x+1, y);
	}
}

