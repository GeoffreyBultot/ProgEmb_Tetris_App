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
		x_pos = 8*X + OFFSET;
		for( i = 0 ; i < 8 ; i++ )
		{
			x_pos++;
			WriteCommand( x_pos & 0x0F);				// set the column (PAGE X): lower 4 bits
			WriteCommand( 0x10 | ((x_pos>>4)&0x0F)); // set the column: (PAGE X) upper 4 bits
			
			if( ((i == 0) && (X==0)) || ((i == 6) && (X==15)) ) //draw horizontal borders
			{
				WriteData(0xFF);
			}
			else//Write normal block
			{
				if(value==0 || i==0 || i == 7) //no pixel if value = 0 or if "i" is the first or last bit of x pos block
					pixels = (C_OLED_BLOCK_BITS_ER);
				else
					pixels = (C_OLED_BLOCK_BITS_SET);
				
				if( Y == 0)
					pixels |= 0b00000001; //Left border
				else if (Y == 7)
					pixels |= 0b10000000; //Right border
				WriteData(pixels);
			}
		}
	}
	
}


void Tetris_DrawEmptyBlock(BYTE X, BYTE Y)
{
	unsigned char i;
	unsigned char x_pos;
	if(X <= C_TETRIS_HEIGHT && Y<= C_TETRIS_WIDTH)
	{	
		//SET Y PAGE ADDRESS // 0xB0 | ((Y>>3)&0x07)
		WriteCommand( 0xB0 | Y );
		x_pos = 8*X + OFFSET;
		for( i = 0 ; i < 8 ; i++ )
		{
			x_pos++;
			WriteCommand( x_pos & 0x0F);				// set the column (PAGE X): lower 4 bits
			WriteCommand( 0x10 | ((x_pos>>4)&0x0F)); // set the column: (PAGE X) upper 4 bits
			
			if( (i == 0) || (i == 7) ) //draw horizontal borders
			{
				WriteData(0xFF);
			}
			else//Write normal block
			{
				if(i%2)
					WriteData(0b10000001);//0b10101011);//(C_OLED_BLOCK_BITS_ER);
				else
					WriteData(0b10000001);//(C_OLED_BLOCK_BITS_ER);
			}
		}
	}
	
}