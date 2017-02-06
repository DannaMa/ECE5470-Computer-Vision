#include "VisXV4.h"          /* VisionX structure include file       */
#include "Vutil.h"           /* VisionX utility header files         */
#include "math.h"


VXparam_t par[] =            /* command line structure               */
{
{    "if=",    0,   " input file  v3dmean: compute local mean"},
{    "of=",    0,   " output file "},
{    "-v",     0,   " visible flag"},
{     0,       0,    0}
};
#define  IVAL   par[0].val
#define  OVAL   par[1].val
#define  VFLAG  par[2].val

int
main(argc, argv)
int argc;
char *argv[];
{
V3fstruct (im);
V3fstruct (tm);
int        x,y,z;               /* index counters                 */
int        xx,yy,zz;            /* window index counters          */
float 	   Sx,Sy,Sz;
float 	   mag;			/* magnitue fo the new pixel	  */
int GxMask[3][3][3];		// Sobel mask in the x direction
int GyMask[3][3][3];		// Sobel mask in the y direction
int GzMask[3][3][3];		// Sobel mask in the z direction

/*Declare Sobel Masks for x direction*/
GxMask[0][0][0] = -1; GxMask[0][0][1] = -2; GxMask[0][0][2] = -1;
GxMask[0][1][0] = -2; GxMask[0][1][1] = -4; GxMask[0][1][2] = -2;
GxMask[0][2][0] = -1; GxMask[0][2][1] = -2; GxMask[0][2][2] = -1;

GxMask[1][0][0] = 0; GxMask[1][0][1] = 0; GxMask[1][0][2] = 0;
GxMask[1][1][0] = 0; GxMask[1][1][1] = 0; GxMask[1][1][2] = 0;
GxMask[1][2][0] = 0; GxMask[1][2][1] = 0; GxMask[1][2][2] = 0;

GxMask[2][0][0] = 1; GxMask[2][0][1] = 2; GxMask[2][0][2] = 1;
GxMask[2][1][0] = 2; GxMask[2][1][1] = 4; GxMask[2][1][2] = 2;
GxMask[2][2][0] = 1; GxMask[2][2][1] = 2; GxMask[2][2][2] = 1;

/*Declare Sobel Masks for y direction*/	
GyMask[0][0][0] = 1; GyMask[0][0][1] = 2; GyMask[0][0][2] = 1;
GyMask[0][1][0] = 0; GyMask[0][1][1] = 0; GyMask[0][1][2] = 0;
GyMask[0][2][0] =-1; GyMask[0][2][1] =-2; GyMask[0][2][2] =-1;

GyMask[1][0][0] = 2; GyMask[1][0][1] = 4; GyMask[1][0][2] = 2;
GyMask[1][1][0] = 0; GyMask[1][1][1] = 0; GyMask[1][1][2] = 0;
GyMask[1][2][0] =-2; GyMask[1][2][1] =-4; GyMask[1][2][2] =-2;

GyMask[2][0][0] = 1; GyMask[2][0][1] = 2; GyMask[2][0][2] = 1;
GyMask[2][1][0] = 0; GyMask[2][1][1] = 0; GyMask[2][1][2] = 0;
GyMask[2][2][0] =-1; GyMask[2][2][1] =-2; GyMask[2][2][2] =-1;

/*Declare Sobel Masks for z direction*/	
GzMask[0][0][0] = -1; GzMask[0][0][1] =  0; GzMask[0][0][2] =  1;
GzMask[0][1][0] = -2; GzMask[0][1][1] =  0; GzMask[0][1][2] =  2;
GzMask[0][2][0] = -1; GzMask[0][2][1] =  0; GzMask[0][2][2] =  1;

GzMask[1][0][0] = -2; GzMask[1][0][1] =  0; GzMask[1][0][2] =  2;
GzMask[1][1][0] = -4; GzMask[1][1][1] =  0; GzMask[1][1][2] =  4;
GzMask[1][2][0] = -2; GzMask[1][2][1] =  0; GzMask[1][2][2] =  2;

GzMask[2][0][0] = -1; GzMask[2][0][1] =  0; GzMask[2][0][2] =  1;
GzMask[2][1][0] = -2; GzMask[2][1][1] =  0; GzMask[2][1][2] =  2;
GzMask[2][2][0] = -1; GzMask[2][2][1] =  0; GzMask[2][2][2] =  1;

    VXparse(&argc, &argv, par); /* parse the command line         */

    V3fread( &im, IVAL);        /* read 3D image                  */
    if ( im.type != VX_PBYTE || im.chan != 1) { /* check  format  */
       fprintf (stderr, "image not byte type or single channel\n");
       exit (1);
    }   
   
    V3fembed(&tm, &im, 1,1,1,1,1,1); /* temp image copy with border */
    if(VFLAG){
       fprintf(stderr,"bbx is %f %f %f %f %f %f\n", im.bbx[0],
                 im.bbx[1],im.bbx[2],im.bbx[3],im.bbx[4],im.bbx[5]);
    }

   for (z = im.zlo; z <= im.zhi; z++) {/* for all pixels */
      for (y = im.ylo; y <= im.yhi; y++) {
        for (x = im.xlo; x <= im.xhi; x++) {
             Sx = 0.0;
	     Sy = 0.0;
	     Sz = 0.0;
	     mag = 0.0;

            for (zz = -1; zz <= 1; zz++) {/* compute the function */
               for (yy = -1; yy <= 1; yy++) {
                 for (xx = -1; xx <= 1; xx++) {
			Sx+= GxMask[zz+1][yy+1][xx+1]*tm.u[z+zz][y+yy][x+xx];
			Sy+= GyMask[zz+1][yy+1][xx+1]*tm.u[z+zz][y+yy][x+xx];
			Sz+= GzMask[zz+1][yy+1][xx+1]*tm.u[z+zz][y+yy][x+xx];
		 }   
               }   
             }
		Sx=Sx/27;
		Sy=Sy/27;
		Sz=Sz/27;
		mag = sqrt(Sx*Sx+Sy*Sy+Sz*Sz);
		if (mag>=40.0) im.u[z][y][x] = 200;
		else im.u[z][y][x] = 100;
        }
      }
   }
   V3fwrite (&im, OVAL);
   exit(0);
}
