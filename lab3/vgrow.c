/*******************************************************************/
/* vgrowc     Compute local max operation on a single byte image   */
/*******************************************************************/

#include "VisXV4.h"           /* VisionX structure include file    */
#include "Vutil.h"            /* VisionX utility header files      */
#include "math.h"

VXparam_t par[] =             /* command line structure            */
{ /* prefix, value,   description                         */   
{    "if=",    0,   " input file  vtemp: local max filter "},
{    "of=",    0,   " output file "},
{    "r=",    0,   " range input "},
{    "-p",     0,  "(verbose) using value of the first pixel in the region as label vaue"},
{     0,       0,   0} /* list termination */
};
#define  IVAL   par[0].val
#define  OVAL   par[1].val
#define  RVAL   par[2].val
#define  PFLAG  par[3].val

void setlabel(int, int, int);
Vfstruct (im);                      /* i/o image structure          */
Vfstruct (tm);                      /* temp image structure         */

int r;//input value that sets the region pixel range
int n=1;//label value
int y,x;          	            /* index counters               */
int first; //value of the first pixel in the region
main(argc, argv)
int argc;
char *argv[];
{

  VXparse(&argc, &argv, par);       /* parse the command line       */
  Vfread(&im, IVAL);                /* read image file              */
  Vfembed(&tm, &im, 1,1,1,1);       /* image structure with border  */

if (RVAL) r = atoi(RVAL);  /* if r= was specified, get value */
if (r < 0 || r > 255) {
	fprintf(stderr, "r= must be between 0 and 255\nUsing r=10\n");
        r = 5;
    }
 
  if ( im.type != VX_PBYTE ) {      /* check image format           */
     fprintf(stderr, "vtemp: no byte image data in input file\n");
     exit(-1);
  }

 for (y = im.ylo ; y <= im.yhi ; y++){
     for (x = im.xlo; x <= im.xhi; x++)
           im.u[y][x] = 0;     /*clear im*/
   }
        
		
if(PFLAG){//flag set.use the first pixel value in the region as label		
  for (y = im.ylo ; y <= im.yhi ; y++) {               /* Search the input image*/
     for (x = im.xlo; x <= im.xhi; x++){
          if(tm.u[y][x]!=0 )//image pixel is not labeled
          {	
		first = tm.u[y][x]; //set first to pixel value	
		 fprintf(stderr, " first = %d\n",first);	
		if(first>255) 
		    first-=255;//label number go back to 1 after reaching 255			
		if (im.u[y][x]==0){
		setlabel( x, y, first); 
		first++;
		}
 	  }
    }
  }
}
else{
  for (y = im.ylo ; y <= im.yhi ; y++) {               /* Search the input image*/
     for (x = im.xlo; x <= im.xhi; x++){
          if(tm.u[y][x]!=0 )//image pixel is not labeled
          {	
		if (im.u[y][x]==0){
		if(n>255) 
		    n-=255;//label number go back to 1 after reaching 255			
		setlabel(x, y, n);
		n++;
		fprintf(stderr, "recursive  n=%d\n",n); // for testing purpose
		}
	  }
     }
  }
}
   Vfwrite(&im, OVAL);             /* write image file*/
   exit(0);
}

  void setlabel(int x, int y, int n)  /*recursive function*/
   {  
	 im.u[y][x] = n;					
     	 if((tm.u[y+1][x]!=0) && (im.u[y+1][x]==0) && (abs(tm.u[y+1][x]-n)<=r)) 
     	 setlabel(x,y+1,n);
     	 if((tm.u[y-1][x]!=0) && (im.u[y-1][x]==0) && (abs(tm.u[y-1][x]-n)<=r)) 
     	 setlabel(x,y-1,n);  
     	 if((tm.u[y][x-1]!=0) && (im.u[y][x-1]==0) && (abs(tm.u[y][x-1]-n)<=r)) 
     	 setlabel(x-1,y,n); 
     	 if((tm.u[y][x+1]!=0) && (im.u[y][x+1]==0) && (abs(tm.u[y][x+1]-n)<=r))
     	 setlabel(x+1,y,n); 
	 
     fprintf(stderr, "setlabel called=%d %d %d\n",x,y,n);//for testing purpose
  }

 
