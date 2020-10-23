//#include "stdio.h"
#include "./headers/Tetris_PIC18.h"
#include "./headers/Tetris_Oled.h"
#include "./headers/Tetris_BMA150.h"
#include "./headers/Oled.h"
#include "./headers/ArraysImages.h"
#include "GenericTypeDefs.h"
#include "timers.h"

typedef enum{
	E_Figure_Erase = 1,
	E_Figure_Set
}T_Figure_Drawing;

static BOOL tb_TetrisGrid[C_TETRIS_HEIGHT][C_TETRIS_WIDTH];
static int py = -1;
static int px = 4;
static int score = 0;
static int level = 0;
BOOL tb_current_fig[C_BLOCKS_SIZE][C_BLOCKS_SIZE];
static int current_fig = 4;
static int counter_movedown = 0;
static int counter_rotate = 0;
BOOL TetrisInGame  = FALSE;
/*Local Prototypes*/
static void Tetris_DrawFigure(BOOL piece[C_BLOCKS_SIZE][C_BLOCKS_SIZE], int x, int y, T_Figure_Drawing Set_or_Erase);


void ShowAnimationStart(void)
{
	unsigned char i,j;
	oledPutImage( tb_uc_LOGO,128,8,0,0);
	DelayMs(1000);
	
	
	for(i = OFFSET ; i < SCREEN_HOR_SIZE + OFFSET; i++)
	{
			WriteCommand( i & 0x0F);
			WriteCommand( 0x10 | ((i>>4)&0x0F));
		for(j = 0 ; j < 8 ; j++)
		{
			//TODO//Tetris_WritePixel(i,j,0);
			
			WriteCommand( 0xB0 | j );
			WriteData(0);
		}
		DelayMs(2);
	}
	DelayMs(50);
}

void Rotate()
{
	BOOL tb_temp_fig[C_BLOCKS_SIZE][C_BLOCKS_SIZE];
	int i,j,k;
	Tetris_EraseFigure(tb_current_fig, px, py);
	/*for(i=0;i<C_BLOCK_SIZE;i++)
	{
		for(i=0;i<C_BLOCK_SIZE;i++)
		{
			tb_temp_fig[i][j] = tb_current_fig[i][j];
		}
	}*/
	
	for (i = 0; i < C_BLOCKS_SIZE; i++)
	{
		for (j = 0; j < C_BLOCKS_SIZE ; j++)
		{
			//umFig[j][i] = currentPiece.umBlock[4 - i - 1][j];
			tb_temp_fig[i][j] = tb_current_fig[ C_BLOCKS_SIZE - j - 1][i];
		}
	}
	
	if (check(tb_temp_fig, px, py))
	{
		for(i=0;i<C_BLOCKS_SIZE;i++)
		{
			for(j=0;j<C_BLOCKS_SIZE;j++)
			{
				tb_current_fig[i][j] = tb_temp_fig[i][j];
			}
		}
		Tetris_SetFigure(tb_current_fig, px, py);
	}
	
}
BOOL moveDown() //TODO: return bool value id movedown is possible
{
	int count = 0;
	int lines = 0;
	BOOL full = TRUE;
	int i,j,k;
	
	Tetris_EraseFigure(tb_current_fig, px, py);
	if(check(tb_current_fig,px,py+1))
	{
		py++;
		Tetris_SetFigure(tb_current_fig, px, py);
		return TRUE;
	}
	else
	{	
		Tetris_SetFigure(tb_current_fig, px, py);
	}
	
	//Si on ne sait plus la placer, (il y a le return true si oui) soit on est dans le cas où on a perdu soit on est dans le cas où la pièce est placée et donc on vérifie si il y a une ligne complète
	//C_TETRIS_HEIGHT = nuwroms	
	//C_TETRIS_WIDTH = numcols
	for (j = C_TETRIS_HEIGHT-1 ; j >= 0 ; j--)
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
	if (lines % 20 == 0 && lines != 0 && (level - 1) * 20 != lines) level++;
	//Score en fonction du nombre de lignes d'un coup
	score += (10 * count * count);
	
	setFigure();
	return FALSE;
}

void moveRight(void)
{
	Tetris_EraseFigure(tb_current_fig, px, py);
	if(check(tb_current_fig,px+1,py))
	{
		px++;
	}
	Tetris_SetFigure(tb_current_fig, px, py);

}

void moveLeft(void)
{
	Tetris_EraseFigure(tb_current_fig, px, py);
	if(check(tb_current_fig,px-1,py))
	{
		px--;
	}
	Tetris_SetFigure(tb_current_fig, px, py);
}

void setFigure()
{
	
	int r;      // Returns a pseudo-random integer between 0 and RAND_MAX.
	int i,j;
	//srand(current_fig );   // Initialization, should only be called once.
	

	T0CONbits.TMR0ON = 0; // Stop the timer
	current_fig = TMR0L + TMR0L ;
	current_fig%=7;
	T0CONbits.TMR0ON = 1; // Stop the timer

	for(i=0;i<C_BLOCKS_SIZE;i++)
	{
		for(j=0;j<C_BLOCKS_SIZE;j++)
		{
			tb_current_fig[i][j]= Blocks[current_fig][i][j];//1;
		}
	}
	py = -1;
    px = 2;
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
	setFigure();
	score = 0;
	level = 0;
}


void Tetris_process(void)
{
	signed int value_x,value_y;
	my_BMA150_XYZ posAcc;
		unsigned int w1;
	if(TetrisInGame )
	{
		get_XYZ(&posAcc);
		counter_movedown ++;
		value_x = posAcc.x - offsetAccX;
		if(counter_movedown <= 9)
		{
			if(counter_movedown%3 == 0)
			{
				w1 = mTouchReadButton(0); //Accept Button
				if (w1 < 600)
				{
					Rotate();
				}
			}
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
				
				if (check(tb_current_fig, px, py+1))
					Tetris_SetFigure(tb_current_fig, px, py);
				else
				{ 	
					Tetris_SetFigure(tb_current_fig, px, py);
					stop();
					return;
				}
			}
			//moveDown();
			counter_movedown=0;
		}
		
		Show_TetrisGrid();
		DelayMs(5);
	}
}

void stop()
{
	counter_movedown=0;	
	TetrisInGame  = FALSE;
	oledPutImage( tb_uc_GameOver,128,8,0,0);
	setScore(score,2,1);
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
				if( (i==15) && j <= 3)
				{
					setScore(score,0,15);
				}
				else
				{
					Tetris_EraseBlock(j, i);
				}
			}
		}
	}

	
}

void Tetris_EraseFigure(BOOL piece[C_BLOCKS_SIZE][C_BLOCKS_SIZE], int x, int y)
{
	Tetris_DrawFigure(piece, x, y,E_Figure_Erase );
}

void Tetris_SetFigure(BOOL piece[C_BLOCKS_SIZE][C_BLOCKS_SIZE], int x, int y)
{
	Tetris_DrawFigure(piece, x, y,E_Figure_Set);
}


static void Tetris_DrawFigure(BOOL piece[C_BLOCKS_SIZE][C_BLOCKS_SIZE], int x,  int y, T_Figure_Drawing Set_or_Erase)
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



BOOL check(BOOL piece[C_BLOCKS_SIZE][C_BLOCKS_SIZE], int x, int y)
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



//x = range(0 to 7) y = range (0 to 15)
void setScore(int score, int startx, int y)
{
	int i;
	int divider=1000;
	int digit;
	for(i = startx; i < startx+4 ; i++)
	{
		digit = (score/divider)%10;
		oledPutImage( NumberFont[digit],8,i+1,8*y,i);
		divider /=10;
	}
	//oledPutROMString((ram unsigned char *)buffer,8*y,2);
	//oledPutImage(buffer,8,1,8,3);
	
}