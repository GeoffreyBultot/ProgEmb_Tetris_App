//#include "stdio.h"
#include "./headers/Tetris_PIC18.h"
#include "./headers/Tetris_Oled.h"
#include "./headers/Tetris_BMA150.h"
#include "./headers/Oled.h"
#include "./headers/ArraysImages.h"
#include "GenericTypeDefs.h"

//#include "./headers/Tetris_BMA150.h"
//#include "./headers/"
//#include <time.h>
#include <stdlib.h>

typedef enum{
	E_Figure_Erase = 1,
	E_Figure_Set
}T_Figure_Drawing;

static BOOL tb_TetrisGrid[C_TETRIS_HEIGHT][C_TETRIS_WIDTH];
static int py = -1;
static int px = 4;
static int current_fig = 0;
static int counter_movedown = 0;
BOOL TetrisInGame  = FALSE;
/*Local Prototypes*/
static void Tetris_DrawFigure(const BOOL piece[C_BLOCKS_SIZE][C_BLOCKS_SIZE], int x, int y, T_Figure_Drawing Set_or_Erase);


void ShowAnimationStart(void)
{
	unsigned char i,j;
	oledPutImage( tb_uc_LOGO,128,8,0,0);
	DelayMs(1000);
	
	for(i = 0 ; i < 128 ; i++)
	{
		for(j = 0 ; j < 64 ; j++)
		{
			Tetris_WritePixel(i,j,0);
		}
	}
	DelayMs(50);
	/*oledPutROMString("TETRIS",0,50);
	oledPutROMString("Powered on PIC18F",2,0);
	oledPutROMString("Generated at ISIB",3,0);
	oledPutROMString("Bultot G. MA1 ELN",4,0);
	oledPutROMString("PRESS MENU TO START",6,5);
	DelayMs(100);
	FillDisplay(0x00);*/
}

BOOL moveDown() //TODO: return bool value id movedown is possible
{
	int count = 0;
	int lines = 0;
	BOOL full = TRUE;
	int i,j,k;
	
	Tetris_EraseFigure(Blocks[current_fig], px, py);
	if(check(Blocks[current_fig],px,py+1))
	{
		py++;
		Tetris_SetFigure(Blocks[current_fig], px, py);
		return TRUE;
	}
	else
	{	
		Tetris_SetFigure(Blocks[current_fig], px, py);
	}
	
	//Si on ne sait plus la placer, (il y a le return true si oui) soit on est dans le cas où on a perdu soit on est dans le cas où la pièce est placée et donc on vérifie si il y a une ligne complète
	//C_TETRIS_HEIGHT = nuwroms	
	//C_TETRIS_WIDTH = numcols
	for (j = C_TETRIS_HEIGHT ; j > 0 ; j--)
	{
		full = TRUE;
		for (i = 0; i < C_TETRIS_WIDTH; i++)
		{
			if (tb_TetrisGrid[j][i] == 0){ full = FALSE ;}
		}
		if (full)
		{
			lines++;
			count++;
			//for (k = j; k > 0; k--){
			
			for(k=j;k < C_TETRIS_HEIGHT-1 ;k++){
				for (i = 0; i < C_TETRIS_WIDTH; i++){
					tb_TetrisGrid[k][i] = tb_TetrisGrid[k+1][i];
				}
			}
		}
	
		full = TRUE;
	}
	
	/*if (lines % 20 == 0 && lines != 0 && (level - 1) * 20 != lines) level++;
	//Score en fonction du nombre de lignes d'un coup
	if (count == 1) score += (10 * count);
	if (count == 2) score += (20 * count);
	if (count == 3) score += (30 * count);
	if (count == 4) score += (40 * count);
	*/
	
		setFigure();
		return FALSE;
	
	
	//DelayMs(5);
}

void moveRight(void)
{
	Tetris_EraseFigure(Blocks[current_fig], px, py);
	if(check(Blocks[current_fig],px+1,py))
	{
		px++;
	}
	Tetris_SetFigure(Blocks[current_fig], px, py);

}

void moveLeft(void)
{
	Tetris_EraseFigure(Blocks[current_fig], px, py);
	if(check(Blocks[current_fig],px-1,py))
	{
		px--;
	}
	Tetris_SetFigure(Blocks[current_fig], px, py);
}

void setFigure()
{
	//srand(time(NULL));   // Initialization, should only be called once.
	//int r = rand();      // Returns a pseudo-random integer between 0 and RAND_MAX.

	current_fig ++;
	current_fig%=7;
	py = -1;
    px = 4;
	//TODO: calcule time intervzal //_timer.Interval = (int)(200 / (level * 0.7));
}
void Tetris_Init(void)
{
	unsigned char i , j;
	TetrisInGame  = TRUE;
	for( i = 0 ; i < C_TETRIS_HEIGHT ; i++)
	{
		for( j = 0 ; j < C_TETRIS_WIDTH ; j++)
		{
			tb_TetrisGrid[i][j] = 0;
		}
	}
}


void Tetris_process(void)
{
	signed int value_x,value_y;
	my_BMA150_XYZ posAcc;
	if(TetrisInGame )
	{
		get_XYZ(&posAcc);
		counter_movedown ++;
		value_x = posAcc.x;
		if(counter_movedown <= 20)
		{
			if(value_x & 0x0100) //negative => Left
			{
				
				value_x = (~(value_x-1))&0x1FF;
				if(value_x > 15)
				{
					moveLeft();
				}
			}
			else //Positive => Right
			{
				if(value_x > 15)
				{
					moveRight();
				}
			}
		
		}
		else	
		{
			if(moveDown() == FALSE)
			{
				//Si on ne sait pas placer la pièce, il y a 2 hypothèse.
				//La première, il n'y a plus de place donc le joueur perd.
				//La deuxième est que la pièce est placée dans le fond de la grille/ sur une autre pièce et du coup, moveDown aura remis py = -1. on vérifie alors 
				//Si la pièce peut se placeer en py + 1. Si elle peut, c'est que py = -1 parce qu'on est arrivé au fond et si non, c'est qu'il n'y a plus de place
				//Console.WriteLine(py);
				
				if (check(Blocks[current_fig], px, py+1))
					Tetris_SetFigure(Blocks[current_fig], px, py);
				else
				{ 
					stop();
					return;
				}
			}
			//moveDown();
			counter_movedown=0;
		}
		
		Show_TetrisGrid();
	}
}

void stop()
{
	counter_movedown=0;	
	TetrisInGame  = FALSE;
	oledPutImage( tb_uc_GameOver,128,8,0,0);
}

void Show_TetrisGrid(void)
{
	unsigned char i , j;
	for( i = 0 ; i < C_TETRIS_HEIGHT ; i++)
	{
		for( j = 0 ; j < C_TETRIS_WIDTH ; j++)
		{
			if(tb_TetrisGrid[i][j])
			{
				Tetris_SetBlock(j, i);
			}
			else
			{
				Tetris_EraseBlock(j, i);
			}
		}
	}
}

void Tetris_EraseFigure(const BOOL piece[C_BLOCKS_SIZE][C_BLOCKS_SIZE], int x, int y)
{
	Tetris_DrawFigure(piece, x, y,E_Figure_Erase );
}

void Tetris_SetFigure(const BOOL piece[C_BLOCKS_SIZE][C_BLOCKS_SIZE], int x, int y)
{
	Tetris_DrawFigure(piece, x, y,E_Figure_Set);
}


static void Tetris_DrawFigure(const BOOL piece[C_BLOCKS_SIZE][C_BLOCKS_SIZE], int x,  int y, T_Figure_Drawing Set_or_Erase)
{
	unsigned char i,j;
	for(i = 0;i< C_BLOCKS_SIZE;i++)
	{
		for(j = 0;j< C_BLOCKS_SIZE;j++)
		{
			if(piece[i][j])
			{
				if(Set_or_Erase == E_Figure_Erase)
				{
					tb_TetrisGrid[15-(y+i)][x+j] = 0;
				}
				else if(Set_or_Erase == E_Figure_Set)
				{
					tb_TetrisGrid[15-(y+i)][x+j] = 1;
				}	
			}
		}
	}
}



BOOL check(const BOOL piece[C_BLOCKS_SIZE][C_BLOCKS_SIZE], int x, int y)
{
	
	//int sz = figure.size;
	int i,j;
	//tb_TetrisGrid[0][1] = 1;
	for (i = 0; i < C_BLOCKS_SIZE; i++)
	{
		for (j = 0; j< C_BLOCKS_SIZE; j++)
		{
			int rx = j + x;
			int ry = 15 - (i + y);
			//C_TETRIS_HEIGHT = nuwroms	
			//C_TETRIS_WIDTH = numcols
			//TODO fix the bug ! Quand une piece de 3 de haut descend, elle ne descend pas tout en bas (logique en sois)
			
			//if(piece[j][i]
			if ( (rx < 0 || rx >= C_TETRIS_WIDTH || ry < 0 || ry >= C_TETRIS_HEIGHT) && (piece[i][j] != 0))
				return FALSE;

			if (!(rx < 0 || rx >= C_TETRIS_WIDTH || ry < 0 || ry >= C_TETRIS_HEIGHT))
			{
				if (piece[i][j])
				{
					if ( tb_TetrisGrid[ry][rx] != 0)
						return FALSE;
				}
			}
		}	
	}
	return TRUE;
}







/*
static void Tetris_DrawFigure(const unsigned char piece[C_BLOCKS_SIZE][C_BLOCKS_SIZE], unsigned char x, unsigned char y, T_Figure_Drawing Set_or_Erase)
{
	unsigned char i,j;
	for(i = 0;i< C_BLOCKS_SIZE;i++)
	{
		for(j = 0;j< C_BLOCKS_SIZE;j++)
		{
			if(piece[i][j])
			{
				if(Set_or_Erase == E_Figure_Erase)
				{
					Tetris_EraseBlock(x+i, y+j);
				}
				else if(Set_or_Erase == E_Figure_Set)
				{
					Tetris_SetBlock(x+i, y+j);
				}
				
			}
			
		}
	}
}*/




