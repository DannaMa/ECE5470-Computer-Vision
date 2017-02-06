/*********************************************************************/
/* vtpeak:     Threshold image between two most sig. hgram peaks     */
/*********************************************************************/

#include "VisXV4.h"          /* VisionX structure include file       */
#include "Vutil.h"           /* VisionX utility header files         */

VXparam_t par[] =            /* command line structure               */
{
{  "if=",   0,   " input file, vtpeak: threshold between hgram peaks"},
{  "of=",   0,   " output file "},
{  "d=",    0,   " min dist between hgram peaks (default 10)"},
{  "-v",    0,   "(verbose) print threshold information"},
{   0,      0,   0} /* list termination */
};
#define  IVAL   par[0].val
#define  OVAL   par[1].val
#define  DVAL   par[2].val
#define  VFLAG  par[3].val

main(argc, argv)
int argc;
char *argv[];
{

    Vfstruct (im);                 /* input image structure          */
    int y,x;                       /* index counters                 */
    int i;
    int th = 0;                        /* input thresh*/	
    int hist[256];                 /* histogram bins                 */
    int thresh;                    /* threshold                      */
    int talv, talv1, talv2;	   /* total gray value               */
    int talp, talp1, talp2;	   /* total number pixel              */
    int avg, avg1, tempavg1, avg2, tempavg2;  /* average gray value   */
    talv = talv1 = talv2 = 0;
    talp = talp1 = talp2 = 0;
    avg = avg1 = tempavg1 = avg2 = tempavg2 = 0;			     
    VXparse(&argc, &argv, par);    /* parse the command line         */

    if (DVAL) th = atoi(DVAL);  /* if d= was specified, get value */
    if (th < 0 || th > 255) {
	fprintf(stderr, "d= must be between 0 and 255\nUsing average gray value\n");
    }

    while ( Vfread( &im, IVAL) ) {
        if ( im.type != VX_PBYTE ) {
              fprintf (stderr, "error: image not byte type\n");
              exit (1);
        }

        /* clear the histogram */
        for (i = 0; i < 256; i++) hist[i] = 0;
 
        /* compute the histogram */
        for (y = im.ylo; y <= im.yhi; y++)
            for (x = im.xlo; x <= im.xhi; x++){
                hist[im.u[y][x]]++;
		talv += im.u[y][x];
		talp++;
  	    }   
         /* compute the average gray value */
	//printf ("%d\n", talv);
	//printf ("%d\n", talp);
	avg = talv/talp;
	//printf ("%d\n", avg);
         /* compute the threshold */
	if (th)
  	thresh = th;
	else
	thresh = avg;
	//printf ("%d\n", thresh);
	 /* compute avg1 and avg2*/
	do {
		//printf ("%d\n", avg1);
		tempavg1 = avg1;
		tempavg2 = avg2;
		for (i = 0; i <= 255; i++){
			if(i < thresh){
				talv1 += i*hist[i];
				talp1 += hist[i];			
			}
			if(i > thresh){
				talv2 += i*hist[i];
				talp2 += hist[i];		
			}
		}
		if (talp1 == 0){
			avg1 = 0;
		}
		else {
			avg1 = talv1/talp1;
		}
		if (talp2 == 0){
			avg2 = 0;
		}
		else {
			avg2 = talv2/talp2;
		}
		thresh = (avg1 + avg2)/2;
	}
	while (tempavg1 != avg1 || tempavg2 != avg2);
	printf ("thresh=%d\n", thresh);
        /* apply the threshold */
        for (y = im.ylo; y <= im.yhi; y++) {
            for (x = im.xlo; x <= im.xhi; x++) {
                 if (im.u[y][x] >= thresh) im.u[y][x] = 255;
                 else                      im.u[y][x] = 0;
            }
        }
  
        Vfwrite( &im, OVAL);
    } /* end of every frame section */
    exit(0);
}
