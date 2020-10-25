#ifndef TETRIS_MTOUCH_H
#define TETRIS_MTOUCH_H

#include "GenericTypeDefs.h"

typedef enum _SCROLL_WHEEL_DIRECTION
{
    NO_TOUCH = 0,
    SCROLL_DOWN,
	SCROLL_UP
}SCROLL_WHEEL_DIRECTION;

typedef enum _SCROLL_WHEEL_POSITION
{
    NO_TOUCH = 0,
    UP,
    DOWN,
    MIDDLE
} SCROLL_WHEEL_POSITION;


extern signed int g_touchStartVal;
extern unsigned char g_isTouched;
extern signed int g_scroolCount;
extern SCROLL_WHEEL_POSITION scrollBarState;

void CalibrateBMA150(void);
BOOL CheckButtonPressed(void);
SCROLL_WHEEL_POSITION GetScrollState(unsigned int w1, unsigned int w2);
SCROLL_WHEEL_DIRECTION GetScrollDirection(void);
#endif