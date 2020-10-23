#include "stdio.h"
#include "Compiler.h"
#include "GenericTypeDefs.h"
#include "./headers/BMA150.h"
#include "./headers/Tetris_BMA150.h"
#include "./headers/HardwareProfile.h"
#include "./headers/Tetris_PIC18.h"
#include "./headers/oled.h"
#include "./headers/Tetris_Oled.h"
#include "./headers/mtouch.h"
#include "./headers/Tetris_Mtouch.h"
#include "./headers/ArraysImages.h"

//#include "mtouch.h"
//#include "soft_start.h"
//#include "Slider2Ch.h"

     #pragma config WDTEN = OFF          //WDT disabled (enabled by SWDTEN bit)
     #pragma config PLLDIV = 3           //Divide by 3 (12 MHz oscillator input)
     #pragma config STVREN = ON            //stack overflow/underflow reset enabled
     #pragma config XINST = OFF          //Extended instruction set disabled
     #pragma config CPUDIV = OSC1        //No CPU system clock divide
     #pragma config CP0 = OFF            //Program memory is not code-protected
     #pragma config OSC = HSPLL          //HS oscillator, PLL enabled, HSPLL used by USB
     #pragma config T1DIG = ON           //Sec Osc clock source may be selected
     #pragma config LPT1OSC = OFF        //high power Timer1 mode
     #pragma config FCMEN = OFF          //Fail-Safe Clock Monitor disabled
     #pragma config IESO = OFF           //Two-Speed Start-up disabled
     #pragma config WDTPS = 32768        //1:32768
     #pragma config DSWDTOSC = INTOSCREF //DSWDT uses INTOSC/INTRC as clock
     #pragma config RTCOSC = T1OSCREF    //RTCC uses T1OSC/T1CKI as clock
     #pragma config DSBOREN = OFF        //Zero-Power BOR disabled in Deep Sleep
     #pragma config DSWDTEN = OFF        //Disabled
     #pragma config DSWDTPS = 8192       //1:8,192 (8.5 seconds)
     #pragma config IOL1WAY = OFF        //IOLOCK bit can be set and cleared
     #pragma config MSSP7B_EN = MSK7     //7 Bit address masking
     #pragma config WPFP = PAGE_1        //Write Protect Program Flash Page 0
     #pragma config WPEND = PAGE_0       //Start protection at page 0
     #pragma config WPCFG = OFF          //Write/Erase last page protect Disabled
     #pragma config WPDIS = OFF          //WPFP[5:0], WPEND, and WPCFG bits ignored 



#define ROM_STRING (rom unsigned char*)



int menuPosition = 0;
int g_menuSelected = 0;
unsigned int w1, w2;


signed short int axisX, axisY, axisZ;

void AppStateMachine(void)
{
	unsigned char i ;
	unsigned int w1, w2;
	unsigned char touchedNow;
	SCROLL_WHEEL_DIRECTION scrollDir;
	
    switch(menuPosition)
    {
        case 0:
			
			//ShowAnimationStart();
			menuPosition++;
			break;
        case 1:
            ResetDevice();  
            FillDisplay(0x00);
			
            oledPutROMString("     TETRIS v1.0     ",0,0);
            oledPutROMString(" By BULTOT  Geoffrey ",1,0);
			oledPutROMString("                     ",2,0);
			oledPutROMString("  Powered on PIC18F  ",3,0);
			oledPutROMString("  Generated at ISIB  ",4,0);
			oledPutROMString("                     ",5,0);
			oledPutROMString(" PRESS MENU TO START ",6,0);
            menuPosition = 2;
            break;

        case 2:
            if(CheckButtonPressed() == TRUE)
            {
                menuPosition=3;
            }
            break;

        case 3:
            oledPutROMString(" 1. Use scroll to    ",0,0);
            oledPutROMString(" select menu items.  ",1,0);
            oledPutROMString(" 2. Tilt the board   ",2,0);
            oledPutROMString("   to move blocks.   ",3,0);
            oledPutROMString(" 3.     Enjoy !      ",4,0);
            oledPutROMString("                     ",5,0);
            oledPutROMString("Press Menu to proceed",6,0);
            menuPosition = 4;
            break;

        case 4:
            if(CheckButtonPressed() == TRUE)
            {
                menuPosition=5;
            }
            break;

        case 5:
			scrollDir = GetScrollDirection();
			
			if (scrollDir == SCROLL_UP)
			{
				if (g_menuSelected > 0)
					g_menuSelected --;
			}
			else if (scrollDir == SCROLL_DOWN)
			{
				if (g_menuSelected < 3)
					g_menuSelected ++;
			}		
			
			
            oledPutROMString("                    ",0,0);
            oledPutROMString("                    ",2,0);
            oledPutROMString("                    ",4,0);

			if (g_menuSelected == 0)
				oledPutROMNegString("      NEW  GAME      ",1,0);	
			else
            	oledPutROMString("      NEW  GAME      ",1,0);
		    
			if (g_menuSelected == 1)
            	oledPutROMNegString("      CALIBRATE      ",3,0);
			else
				oledPutROMString("      CALIBRATE      ",3,0);
			
			if (g_menuSelected == 2)
            	oledPutROMNegString("      QUIT GAME      ",5,0);
			else
				oledPutROMString("      QUIT GAME      ",5,0);

            oledPutROMString("                    ",6,0);

			w1 = mTouchReadButton(0); //Accept Button
			if (w1 < 600)
			{
				switch(g_menuSelected)
				{
					case 0: menuPosition = 6; oledPutImage( tb_uc_PressStart,128,8,0,0); DelayMs(100);  g_menuSelected = 0; break;
					case 1: menuPosition = 9; DelayMs(100); FillDisplay(0x00); g_menuSelected = 0; break;
					case 2: 
						_asm
						RESET
						_endasm
						 break;
				}
			}
            break;
		case 6://Playing
				w1 = mTouchReadButton(0); //Accept Button
				if (w1 < 600)
				{
					Tetris_Init();
					menuPosition = 7;
					FillDisplay(0x00);
				}
			
			break;
		case 7://
				if( TetrisInGame == FALSE)
					menuPosition = 8;
				else
				{
					Tetris_process();
					
				}
			break;
		case 8://
			w1 = mTouchReadButton(0); //Accept Button
			if (w1 < 600)
			{
				FillDisplay(0x00);
				menuPosition = 5;
				DelayMs(100);
			}
			break;
        case 9://CALIBRATION
			CalibrateBMA150();
			w1 = mTouchReadButton(3);
			if (w1 < 600)
			{
				menuPosition = 5;
			}
            break;

		case 10:
			FillDisplay(0xFF);
			/*oledPutROMNegString((ROM_STRING)"  CORNGRATULATIONS!  ",0,0);
			oledPutROMNegString((ROM_STRING)"   You won nothing   ",2,0);
			oledPutROMNegString((ROM_STRING)" You just waste your ",3,0);
			oledPutROMNegString((ROM_STRING)"   PRECIOUS time :)  ",4,0);*/
			/*DelayMs(2000);
			DemoIntroState = 6;
			FillDisplay(0x00);
			g_level = 0;
			g_ballGtt = 0;*/
			break;
        default:
            break;
    }
}


void InitApplication(void)
{
	unsigned int pll_startup_counter = 600;
	
	ResetDevice();
	ADCON1 |= 0x0F;                 // Default all pins to digital

	//PLL
	OSCTUNEbits.PLLEN = 1;  //Enable the PLL and wait 2+ms until the PLL locks before enabling USB module
	while(pll_startup_counter--);

	ANCON0 = 0xFF;                  // Default all pins to digital
    ANCON1 = 0xFF;                  // Default all pins to digital

	
	FillDisplay(0x00);
	InitBma150 ();
	//Tetris_Init();

	/* Initialize the mTouch library */
	mTouchInit();
	/* Call the mTouch callibration function */
	mTouchCalibrate();

	
}
void main (void)
{
	int y = 3;
	InitApplication();
	// 1 DOWN
	// 2 UP
	// 3 L
	// 0 R
	
	while(1)
	{
		AppStateMachine();
	}
}