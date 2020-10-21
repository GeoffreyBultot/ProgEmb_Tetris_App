#include "./headers/Tetris_BMA150.h"
#include "./headers/BMA150.h"


void get_XYZ(my_BMA150_XYZ *posAcc)
{
	posAcc->x = (signed short int)(((unsigned short int)BMA150_ReadByte(BMA150_ACC_X_MSB) << 8) | BMA150_ReadByte(BMA150_ACC_X_LSB));
	posAcc->x /= 32;

	posAcc->y = (signed short int)(((unsigned short int)BMA150_ReadByte(BMA150_ACC_Y_MSB) << 8) | BMA150_ReadByte(BMA150_ACC_Y_LSB));
	posAcc->y /= 32;

	posAcc->z = (signed short int)(((unsigned short int)BMA150_ReadByte(BMA150_ACC_Z_MSB) << 8) | BMA150_ReadByte(BMA150_ACC_Z_LSB));
	posAcc->z /= 32;
/*
	posAcc->y =  (signed int)BMA150_ReadByte(BMA150_ACC_Y_LSB)>>6;
	posAcc->y |= (signed int)BMA150_ReadByte(BMA150_ACC_Y_MSB)<<2;
	posAcc->z =  (signed int)BMA150_ReadByte(BMA150_ACC_Z_LSB)>>6;
	posAcc->z |= (signed int)BMA150_ReadByte(BMA150_ACC_Z_MSB)<<2;*/
}