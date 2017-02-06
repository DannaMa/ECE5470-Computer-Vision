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
//void lmax(int, int);
void setlabel(int, int, int, int, int);
Vfstruct (im);                      /* i/o image structure          */
Vfstruct (tm);                      /* temp image structure         */

main(argc, argv)
int argc;
char *argv[];
{
int        y,x;                     /* index counters               */
int        r;                       /* region pixel range           */

  VXparse(&argc, &argv, par);       /* parse the command line       */

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
  for (y = 350 ; y <= 684 ; y++) {
     for (x = 360; x <= 630; x++)  {
	im.u[y][x] = tm.u[y][x];
     }
   }
   Vfwrite(&im, OVAL);             /* write image file                */
   exit(0);
}

