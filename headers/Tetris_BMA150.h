#ifndef _TETRIS_BMA150_H
#define _TETRIS_BMA150_H

#include "GenericTypeDefs.h"
typedef struct XYZ
{
	signed int x;
	signed int y;
	signed int z;
} my_BMA150_XYZ ;

extern signed int offsetAccX;
extern signed int offsetAccY;

void get_XYZ(my_BMA150_XYZ *posAcc);
void CalibrateBMA150();
#endif

