#include "./headers/Tetris_Oled.h"
#include "./headers/oled.h"

void Tetris_SetBlock(BYTE X, BYTE Y)
{
	//INVERT THE X Y BECAUSE SCREEN TURNED BY 90°
	Tetris_DrawBlock(Y,X,1);
}
void Tetris_EraseBlock(BYTE X, BYTE Y)
{
	//INVERT THE X Y BECAUSE SCREEN TURNED BY 90°
	Tetris_DrawBlock(Y,X,0);
}

void Tetris_DrawBlock(BYTE X, BYTE Y, BYTE value)
{
	unsigned char i;
	unsigned char x_pos;
	unsigned char pixels = 0x00;
	if(X <= C_TETRIS_HEIGHT && Y<= C_TETRIS_WIDTH)
	{	
		//SET Y PAGE ADDRESS // 0xB0 | ((Y>>3)&0x07)
		WriteCommand( 0xB0 | Y );
		x_pos = 8*X;
		for( i = 0 ; i < 8 ; i++ )
		{
			x_pos++;
			WriteCommand( x_pos & 0x0F);				// set the column (PAGE X): lower 4 bits
			WriteCommand( 0x10 | ((x_pos>>4)&0x0F)); // set the column: (PAGE X) upper 4 bits
			if(i == 0||i==7)
				WriteData(C_OLED_BLOCK_BITS_ER);
			else
			{
				if(value)
					pixels = C_OLED_BLOCK_BITS_SET;
				else
					pixels = C_OLED_BLOCK_BITS_ER;
				
				if(  Y == 0 ) //|| (X == 7)) && (( Y == 0 ) || (Y == 15)) )
					pixels |= 0b00000001;
				
				WriteData(pixels);
			}
		}
	}
	
}