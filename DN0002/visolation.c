/*******************************************************************/
/* visolate isolate ROI-a single tooth with restoration */
/*******************************************************************/
#include "VisXV4.h" /* VisionX structure include file */
#include "Vutil.h" /* VisionX utility header files */
#include <stdio.h>
#include <stdlib.h>

VXparam_t par[] = /* command line structure */
{ /* prefix, value, description */
{ "if=", 0, " input file vtemp: local max filter "},
{ "of=", 0, " output file "},
{ 0, 0, 0} /* list termination */
};

#define IVAL par[0].val
#define OVAL par[1].val

void setlabel(int, int, int, int, int);
Vfstruct (im); /* i/o image structure */
Vfstruct (tm); /* temp image structure */
main(argc, argv)

int argc;
char *argv[];
{
int y,x; /* index counters */
VXparse(&argc, &argv, par); /* parse the command line */
Vfread(&im, IVAL); /* read image file */
Vfembed(&tm, &im, 1,1,1,1); /* image structure with border */
if ( im.type != VX_PBYTE ) { /* check image format */
fprintf(stderr, "vtemp: no byte image data in input file\n");
exit(-1);
}

/* initialize the image */
for (y = im.ylo ; y <= im.yhi ; y++) {
   for (x = im.xlo; x <= im.xhi; x++) {
	im.u[y][x] = 0;
    }
}

/* find the location of the tooth copy to the new image*/
for (y = 38 ; y <= 605 ; y++) {
   for (x = 647; x <= 1040; x++) {
	im.u[y][x] = tm.u[y][x];
   }
}

Vfwrite(&im, OVAL); /* write image file */
exit(0);
}
