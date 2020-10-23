#ifndef _TETRIS_OLED_H
#define _TETRIS_OLED_H


#include "GenericTypeDefs.h"

#define	oledWR			LATEbits.LATE1
#define	oledWR_TRIS		TRISEbits.TRISE1
#define	oledRD			LATEbits.LATE0
#define	oledRD_TRIS		TRISEbits.TRISE0
#define	oledCS			LATEbits.LATE2
#define	oledCS_TRIS		TRISEbits.TRISE2
#define	oledRESET		LATDbits.LATD1
#define	oledRESET_TRIS	TRISDbits.TRISD1
#define	oledD_C			LATBbits.LATB5
#define	oledD_C_TRIS	TRISBbits.TRISB5

#define C_OLED_BLOCK_BITS_SET 0b01111110
#define C_OLED_BLOCK_BITS_ER 0b00000000
#define C_TETRIS_WIDTH 8
#define C_TETRIS_HEIGHT 16
void stop();
/*void Tetris_Oled_Write(char data);
char Tetris_Oled_Read();
void Tetris_WritePixel(BYTE X, BYTE Y, BYTE Data);
*/void Tetris_DrawBlock(BYTE X, BYTE Y, BYTE value);

void Tetris_SetBlock(BYTE X, BYTE Y);
void Tetris_EraseBlock(BYTE X, BYTE Y);

void Tetris_DrawEmptBlock(BYTE X, BYTE Y);
#endif
