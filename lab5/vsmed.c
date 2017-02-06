/*********************************************************************/
/* vssum   Compute local 1x1xn mean using the buffer method          */
/*********************************************************************/

#include "VisXV4.h"          /* VisionX structure include file       */
#include "Vutil.h"           /* VisionX utility header files         */

VXparam_t par[] =            /* command line structure               */
{
{    "if=",    0,   " input file vssum: compute temporal mean"},
{    "of=",    0,   " output file "},
{    "n=",     0,   " number of frames "},
{     0,       0,    0}
};
#define  IVAL   par[0].val
#define  OVAL   par[1].val
#define  NVAL   par[2].val

int
main(argc, argv)
int argc;
char *argv[];
{
V3fstruct (im);
V3fstruct (tm);
int        x,y,z;           /* index counters                 */
int        n;               /* Number of frames to average    */
int        med;
int a[3];
    VXparse(&argc, &argv, par); /* parse the command line    */

 n = (NVAL ? atoi(NVAL) : 3); /* read n, default is n=3   */

    while (Vbfread( &im, IVAL, n)) {
	if ( im.type != VX_PBYTE || im.chan != 1) { /* check format  */
           fprintf (stderr, "image not byte type\n");
           exit (1);
        }
int a,b,c;
a=b=c=0;
        for (y = im.ylo; y <= im.yhi; y++) {
           for (x = im.xlo; x <= im.xhi; x++) {
              med = 0;
              for (z = im.zlo; z <= im.zhi; z++) {
                a = im.u[0][y][x];
		b = im.u[1][y][x];
		c = im.u[2][y][x];
		med = ((a-b)*(b-c) > -1 ? b : ((a-b)*(a-c) < 1 ? a : c));
		printf ("a=%d,b=%d,c=%d, z=%d,med=%d \n",a,b,c,z,med);
              }       
              im.u[0][y][x] = med;
            }
        }
        V3fwrite (&im, OVAL); /* write the oldest frame */
    }
    exit(0);
}
