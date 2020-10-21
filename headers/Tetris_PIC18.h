#ifndef _TETRIS_PIC18_H
#define _TETRIS_PIC18_H
#include "./headers/Tetris_Pieces.h"
void showAnimationStart(void);
/*void Tetris_DrawFigure(const unsigned char piece[C_BLOCKS_SIZE][C_BLOCKS_SIZE], unsigned char x, unsigned char y);
void Tetris_EraseFigure(const unsigned char piece[C_BLOCKS_SIZE][C_BLOCKS_SIZE], unsigned char x, unsigned char y);
*/

extern BOOL TetrisInGame ;
void Tetris_Init(void);
void stop(void);
void ShowAnimationStart(void);
void Show_TetrisGrid(void);
BOOL moveDown(void);
void moveRight(void);
void moveLeft(void);
void Rotate(void);
void setFigure(void);
void Tetris_process(void);
BOOL check(const BOOL piece[C_BLOCKS_SIZE][C_BLOCKS_SIZE], int x, int y);
void Tetris_EraseFigure(const BOOL piece[C_BLOCKS_SIZE][C_BLOCKS_SIZE], int x, int y);
void Tetris_SetFigure(const BOOL piece[C_BLOCKS_SIZE][C_BLOCKS_SIZE], int x, int y);
#endif