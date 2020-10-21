#include "./headers/Tetris_Oled.h"
#include "./headers/oled.h"

void Tetris_Oled_Write(char data)
{								// CS is still active from the OledRMW_Read function
								// The Write line is inactive=1
    TRISD = 0x00;				// Start the Write, make PortD = output.
    LATD  = data;				// output the new value
    oledWR = 0;					// toggle the Write line to re-write the data
    oledWR = 1;
								// we can leave the chip selected for the following END command
	WriteCommand( 0xEE );		// End the Read/Modify/Write;
								// the WriteCommand leaves the DisplayController=DeSelected
								// and PortD=Input
}
char Tetris_Oled_Read(void)			// This performs the Read of a Read/Modify/Write 
{									
	char	btemp;
								// the Column address & Page have already been set!
	WriteCommand( 0xE0 );		// Start a read/modify/write
								// The WriteCommand leaves TRISD=0xFF=input, so we don't have to do it.
	oledD_C = 1;				// Set the control lines for Data
	oledWR = 1;
	oledRD = 1;
	oledCS = 0;					// Select the Display Controller
	oledRD = 0;					// Toggle the RD line for the dummy read
	Nop(); Nop(); Nop(); Nop();	// Delay so the Read can complete
	btemp = PORTD;				// Read The result- this is the Dummy read!
								// just keep the CS active for the second real read
	oledRD = 1;					// Toggle the RD line
	oledRD = 0;					// Assert the read control
	Nop(); Nop(); Nop(); Nop();	// Delay so the Read can complete
	btemp = PORTD;				// The real read
	oledRD = 1;					// deassert the read control and end the read
								// CS is left active until the end of the RMW cycle
	return(btemp);				// return the byte read
}

//**************************** 
// Plotting an X,Y pixel
//****************************
//  The inputs are all unsigned since Max X = 132
void Tetris_WritePixel(BYTE X, BYTE Y, BYTE Data) 
{
	BYTE	oled_column;
	BYTE	this_data;

		// first the X coordinate
	oled_column = X + OFFSET;  						// add the column offset=2 from oled.h
		// first set the X coordinate: the column-low and high nibbles
	WriteCommand( oled_column & 0x0F );				// set the column: lower 4 bits
	WriteCommand( 0x10+((oled_column>>4)&0x0F) );	// set the column: upper 4 bits
	WriteCommand( 0xB0 | ((Y>>3)&0x07) );			// Set the Y page address
			
	this_data = Tetris_Oled_Read();						// do the Read of the Read/Modify/Write

	if(Data)
		this_data = this_data | (0x01<<(Y&0x07)) ;	// set the bit
	else
		this_data = this_data & (!(0x01<<(Y&0x07))) ;	// clear the bit

 	Tetris_Oled_Write( this_data );						// Write the bit and complete the RMW
 
	return;
}


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
	
	if(X <= C_TETRIS_HEIGHT && Y<= C_TETRIS_WIDTH)
	{	
		//SET Y PAGE ADDRESS // 0xB0 | ((Y>>3)&0x07)
		WriteCommand( 0xB0 | Y );
		x_pos = 8*X;
		for( i = 1 ; i < 7 ; i++ )
		{
			x_pos++;
			WriteCommand( x_pos & 0x0F);				// set the column (PAGE X): lower 4 bits
			WriteCommand( 0x10 | ((x_pos>>4)&0x0F)); // set the column: (PAGE X) upper 4 bits
			
			if(value)
				WriteData(C_OLED_BLOCK_BITS_SET);
			else
				WriteData(C_OLED_BLOCK_BITS_ER);
		
			}
		}
	
}