#ifndef _TETRIS_BMA150_H
#define _TETRIS_BMA150_H

#include "GenericTypeDefs.h"
typedef struct XYZ
{
	signed short int x;
	signed int y;
	signed int z;
} my_BMA150_XYZ ;

void get_XYZ(my_BMA150_XYZ *posAcc);

#endif

