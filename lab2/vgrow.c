/*******************************************************************/
/* vtemp      Compute local max operation on a single byte image   */
/*******************************************************************/

#include "VisXV4.h"           /* VisionX structure include file    */
#include "Vutil.h"            /* VisionX utility header files      */

VXparam_t par[] =             /* command line structure            */
{ /* prefix, value,   description                         */   
{    "if=",    0,   " input file  vtemp: local max filter "},
{    "of=",    0,   " output file "},
{     0,       0,   0}  /* list termination */
};
#define  IVAL   par[0].val
#define  OVAL   par[1].val
void setlabel(int, int, int);
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


 for (y = im.ylo ; y <= im.yhi ; y++){
     for (x = im.xlo; x <= im.xhi; x++)
           im.u[y][x] = 0;     /*clear im*/
   }

  int n=1;          /*set the current label to 1*/
  for (y = im.ylo ; y <= im.yhi ; y++) {               /* Search the input image*/
     for (x = im.xlo; x <= im.xhi; x++){
          if((tm.u[y][x]!=0) && (im.u[y][x]==0))
          {setlabel(x, y, n);
		n++;}
     }
   }

   Vfwrite(&im, OVAL);             /* write image file*/
   exit(0);
}

  void setlabel(int x, int y, int n)  /*recursive function*/
   {  im.u[y][x] = n;					
      if((tm.u[y+1][x]!=0)&& (im.u[y+1][x]==0)) 
      setlabel(x,y+1,n);
      if((tm.u[y-1][x]!=0) && (im.u[y-1][x]==0)) 
      setlabel(x,y-1,n);  
      if((tm.u[y][x-1]!=0) && (im.u[y][x-1]==0)) 
      setlabel(x-1,y,n); 
      if((tm.u[y][x+1]!=0) && (im.u[y][x+1]==0))
      setlabel(x+1,y,n); 
  }
