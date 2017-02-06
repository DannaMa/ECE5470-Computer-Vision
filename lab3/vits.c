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

    Vfstruct (im);                 /* input image structure      		 */
    int y,x;                       /* index counters             	         */
    int thresh;                    /* threshold                    		 */
    int sum;			   /* sum of all grey value of the image 	 */
    int avg1, avg2;     	   /* average grey values for R1 and R2          */
    int pcount1,pcount2;	   /* number of pixels in R1,R2 		 */    
    int sum1, sum2; 		   /* sum of all grey value of the image in R1,R2*/
    int prevavg1,prevavg2;         /* previous avg1,avg2 values    	         */

			     
    VXparse(&argc, &argv, par);    /* parse the command line         */

    while ( Vfread( &im, IVAL) ) {
        if ( im.type != VX_PBYTE ) {
              fprintf (stderr, "error: image not byte type\n");
              exit (1);
        }

    if (DVAL) thresh = atoi(DVAL);  /* if threshold value was specified, get value */
    if (thresh < 0 || thresh > 255) {
	fprintf(stderr, "thresh= must be between 0 and 255\nUsing calculated thresh instead\n");
	sum = 0;
		/* compute the inital threshold and use it as an initial threshold*/
		for (y = im.ylo; y <= im.yhi; y++)
		    for (x = im.xlo; x <= im.xhi; x++){
		         sum += im.u[y][x];
			}
		thresh = sum/(im.yhi*im.xhi);
	fprintf (stderr, "x=%d y=%d\n",x,y);		
	fprintf (stderr, "initial thresh=%d\n",thresh);
	}

	
        avg1 = 0;
	avg2 = 0;
	sum1 = 0;
	sum2 = 0;
	pcount1 = 0;
	pcount2 = 0;
	prevavg1 = 0;
	prevavg2 = 0;
	/* compute the inital threshold avg1,avg2 in regions R1,R2 */
	do {	
		prevavg1 = avg1;	
		prevavg2 = avg2;	
		for (y = im.ylo; y <= im.yhi; y++)
		    for (x = im.xlo; x <= im.xhi; x++){
		            if(im.u[y][x]>=thresh){
			       sum1 +=im.u[y][x];
			       pcount1 ++;}
			    else {
			       sum2 +=im.u[y][x];
			       pcount2 ++;}
			}
		/* compute the new threshold */
		if (pcount1 !=0 && pcount2 !=0){
		avg1 = sum1/pcount1;
		avg2 = sum2/pcount2;
		thresh = (avg1 + avg2)/2;}
	}while (prevavg1 !=avg1 || prevavg2 != avg2);
	fprintf (stderr, "x=%d y=%d\n",x,y);
	fprintf(stderr, "avg1 = %d avg2 = %d threshold = %d\n", avg1,avg2,thresh);

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
