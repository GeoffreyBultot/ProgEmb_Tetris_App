#include "./headers/Tetris_BMA150.h"
#include "./headers/BMA150.h"
#include "./headers/mtouch.h"

signed int offsetAccX = 0;
signed int offsetAccY = 0;

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


void CalibrateBMA150()
{
	unsigned int w1;
	signed int axisX;
	signed int axisY; 
	static unsigned int filterPass = 0;
	char buffer[16];
	my_BMA150_XYZ ValueXYZ;
	get_XYZ(&ValueXYZ);
	//ValueXYZ.x += g_devAccelerationX-g_devAccOffsetX;
	//ValueXYZ.y += g_devAccelerationY-g_devAccOffsetY;

	axisX += ValueXYZ.x-offsetAccX;
	axisY += ValueXYZ.y-offsetAccY;

	//axisZ += g_devAccelerationZ-g_devAccOffsetZ;
	
	if (filterPass == 200)
	{
		oledPutROMString("   CALIBRATE SENSOR  ",0,0);
		sprintf((char *)buffer, (const far rom char *)("   X:  %d             "), axisX/200);
		oledPutString((unsigned char *)buffer,2,0);
		sprintf((char *)buffer, (const far rom char *)("   Y:  %d             "), axisY/200);
		oledPutString((unsigned char *)buffer,3,0);
	
		axisX = 0;
		axisY = 0;
		filterPass = 0;
	}
	w1 = mTouchReadButton(0);
	if (w1 < 600)
	{
		offsetAccX = ValueXYZ.x;
		offsetAccY = ValueXYZ.y;
	}
	
	
	filterPass ++;
}