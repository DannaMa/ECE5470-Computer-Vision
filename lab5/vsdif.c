/*********************************************************************/
/* vssum   Compute local 1x1xn mean using the buffer method          */
/*********************************************************************/

#include "VisXV4.h"          /* VisionX structure include file       */
#include "Vutil.h"           /* VisionX utility header files         */
#include <math.h>

VXparam_t par[] =            /* command line structure               */
{
{    "if=",    0,   " input file vssum: compute temporal mean"},
{    "of=",    0,   " output file "},
{    "n=",     0,   " number of frames "},
{    "th=",    0,   "difference threshold"},
{     0,       0,    0}
};
#define  IVAL   par[0].val
#define  OVAL   par[1].val
#define  NVAL   par[2].val
#define  TVAL   par[3].val
int
main(argc, argv)
int argc;
char *argv[];
{
V3fstruct (im);
V3fstruct (tm);
int        x,y,z;           /* index counters                 */
int        n;               /* Number of frames to average    */
int 	   th;
int        change;

    VXparse(&argc, &argv, par); /* parse the command line    */

    n = (NVAL ? atoi(NVAL) : 2); /* read n, default is n=1   */
    th = (TVAL ? atoi(TVAL): 20); /* read th, default is thr=20  */
    while (Vbfread( &im, IVAL, n)) {
	if ( im.type != VX_PBYTE || im.chan != 1) { /* check format  */
           fprintf (stderr, "image not byte type\n");
           exit (1);
        }
        for (y = im.ylo; y <= im.yhi; y++) {
           for (x = im.xlo; x <= im.xhi; x++) {
              change = 0;
             // for (z = im.zlo; z <= im.zhi; z++) {
                  
//              }
		if (abs(im.u[0][y][x]-im.u[1][y][x])> th) change = 255;
		else change = 128; 
              im.u[0][y][x] = change;
            }
        }
        V3fwrite (&im, OVAL); /* write the oldest frame */
    }
    exit(0);
}
