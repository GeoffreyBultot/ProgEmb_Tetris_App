
#include "stdio.h"
#include "Compiler.h"
#include "./headers/Oled.h"
#include "./headers/Tetris_Oled.h"
#include "./headers/mtouch.h"
#include "./headers/Tetris_Mtouch.h"




SCROLL_WHEEL_POSITION scrollBarState = NO_TOUCH;
signed int g_touchStartVal = 0;
unsigned char g_isTouched = 0;
signed int g_scroolCount = 0;

BOOL CheckButtonPressed(void)
{
    static char buttonPressed = FALSE;
    static unsigned long buttonPressCounter = 0;

    if(PORTBbits.RB0 == 0)
    {
        if(buttonPressCounter++ > 2000)
        {
            buttonPressCounter = 0;
            buttonPressed = TRUE;
        }
    }
    else
    {
        if(buttonPressed == TRUE)
        {
            if(buttonPressCounter == 0)
            {
                buttonPressed = FALSE;
                return TRUE;
            }
            else
            {
                buttonPressCounter--;
            }
        }
    }

    return FALSE;
}




SCROLL_WHEEL_POSITION GetScrollState(unsigned int w1, unsigned int w2)
{
	if ((w1 < 700) || (w2 < 700))
	{
		if (w1 < w2)
		{
            return UP;			
		}
		else
		{
            return DOWN;	
		}
	}
	else
	{
		return NO_TOUCH;
	}
}



SCROLL_WHEEL_DIRECTION GetScrollDirection()
{
	unsigned int w1, w2;
	unsigned char buffer[10] = {0};
	w1 = mTouchReadButton(1);
	w2 = mTouchReadButton(2);
	scrollBarState = GetScrollState(w1, w2);
	w1 = 1;

	oledPutROMString(ROM_STRING(buffer),6,0);
	if ((scrollBarState != NO_TOUCH) && (g_isTouched == 0))
	{		
		g_isTouched = 1;
		g_touchStartVal = w1-w2;
	}
	
	if (scrollBarState == NO_TOUCH)
	{
		g_isTouched = 0;
		return NO_TOUCH;
	}

	if (g_isTouched && ((signed int)g_touchStartVal - ((signed int)w1-(signed int)w2)) > 100)
	{
		g_touchStartVal = w1-w2;
		return SCROLL_UP;
		
	}
	else if (g_isTouched && ((signed int)g_touchStartVal - ((signed int)w1-(signed int)w2)) < -100)
	{
		g_touchStartVal = w1-w2;
		return SCROLL_DOWN;
	}	

}