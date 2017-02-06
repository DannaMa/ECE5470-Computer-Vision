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
  VXparse(&argc, &argv, par);       /* parse the command line       */

  Vfread(&im, IVAL);                /* read image file              */
  Vfembed(&tm, &im, 1,1,1,1);       /* image structure with border  */
  if ( im.type != VX_PBYTE ) {      /* check image format           */
     fprintf(stderr, "vtemp: no byte image data in input file\n");
     exit(-1);
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
	if (tm.u[y][x] < 80) {
	    if(tm.u[y+1][x] < 60 || tm.u[y-1][x] < 60 || tm.u[y][x+1] < 60 || tm.u[y][x-1] < 60) {
		setzero(x,y);
	    }
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
	if (tm.u[y+1][x] < 80 && tm.u[y+1][x] != 255) {
		setzero(x, y+1);
	}
	if (tm.u[y-1][x] < 80 && tm.u[y-1][x] != 255) {
		setzero(x, y-1);
	}
	if (tm.u[y][x-1] < 80 && tm.u[y][x-1] != 255) {
		setzero(x-1, y);
	}
	if (tm.u[y][x+1] < 80 && tm.u[y][x+1] != 255) {
		setzero(x+1, y);
	}
}


