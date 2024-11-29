/*
Author: Chase Pitts
Title: rogue-like with Ncurses
Description: Started in 2022 as a C programming review project 
*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <ctype.h>
#define WIDTH 30
#define HEIGHT 10
#define CREATURES 20
#define MAX 40



void testTwo(int * array[MAX][MAX])
{
int i;
int j;
time_t t;
srand((unsigned) time(&t)); //initialize random number generator
for ( i = 0; i < MAX; i++) 
   {
      for( j = 0; j < MAX; j++)
      {
      *array[i][j] = (int) (rand() % 16); //make MAX x MAX matrix, composed of 3s
      }
   }
}

void printArray(int * array[MAX][MAX])
{
int i;
int j;
for ( i = 0; i < MAX; i++) 
   {
      for( j = 0; j < MAX; j++)
      {
      //printf("array[%d][%d] = %d\n", i, j, *array[i][j]); //make MAX x MAX matrix, composed of 3s
      printf("%x", *array[i][j]);
      }
   printf("\n");
   }
}

//generate dungeon from previous seed
void mixer(int * array[MAX][MAX])
{
int i;
int j;
int seedNum;
int interm[4];
for ( i = 0; i < MAX; i=i+2)
   {
      for( j = 0; j < MAX; j=j+2)
      {
      //printf("array[%d][%d] = %d\n", i, j, *array[i][j]); //make MAX x MAX ma>
      seedNum = (*array[i][j]+*array[i+1][j]+*array[i][j+1]+*array[i+1][j+1])/4;
      printf("%x", seedNum);

      //use seedNum to determine output
      interm[0]=(seedNum<6)||(seedNum == 13);
      interm[1]=(seedNum<5)||(seedNum == 13)||(seedNum == 5);
      interm[2]=(seedNum<4)||(seedNum == 8)||(seedNum == 5);
      interm[3]=(seedNum<8)||(seedNum == 8);

      //assign output to array
      *array[i][j]=interm[0];
      *array[i+1][j]=interm[1];
      *array[i][j+1]=interm[2];
      *array[i+1][j+1]=interm[3];
      }
   printf("seeds\n");
   }
}

void printMap(int * array[MAX][MAX])
{
int i;
int j;
for ( i = 0; i < MAX; i++)
   {
      for( j = 0; j < MAX; j++)
      {
      //printf("array[%d][%d] = %d\n", i, j, *array[i][j]); //make MAX x MAX matrix, composed of 3s
      if(*array[i][j] == 1){printf("%c", 35);}
      if(*array[i][j] == 0){printf("%c", 32);}
      }
   printf("\n");
   }
}


//spawn enemy placeholders
void placeEnemies(int * array[MAX][MAX])
{
int i;
int j;
for ( i = 0; i < MAX-2; i++)
   {
      for( j = 0; j < MAX; j++)
      {
      //printf("array[%d][%d] = %d\n", i, j, *array[i][j]); //make MAX x MAX matrix, composed of 3s
      if((*array[i][j] == 0)&&(*array[i+1][j] == 0)&&(*array[i+2][j] == 0)&&(rand() % 12 <2)){*array[i][j] = 3;}
      //place dummy enemy "3" if room and random chance conditions met
      }
   printf("\n");
   }
}

void placeWater(int * array[MAX][MAX])
{
int i;
int j;
for ( i = 0; i < MAX-4; i++)
   {
      for( j = 0; j < MAX-4; j++)
      {
      //printf("array[%d][%d] = %d\n", i, j, *array[i][j]); //make MAX x MAX>
      if((*array[i][j]+*array[i+1][j]+*array[i][j+1]+*array[i+1][j+1] == 0)&&(2>(rand() % 40)))
      {
	//place holder pond array
        *array[i][j]=4;
	*array[i+1][j]=4;
	*array[i][j+1]=4;
	*array[i+1][j+1]=4;
	*array[i+2][j+1]=4;
	*array[i+1][j+2]=4;
	*array[i+2][j+2]=4;
      }
      //place dummy enemy "3" if room and random chance conditions met
      }
   printf("\n");
   }
}


//print map with Ncurses
void printMapc(int * array[MAX][MAX])
{
int i;
int j;
int k;
for ( i = 0; i < MAX; i++) 
   {
      for( j = 0; j < MAX; j++)
      {
      //printf("array[%d][%d] = %d\n", i, j, *array[i][j]); //make MAX x MAX matr>
      if(*array[i][j] == 1){mvprintw(i, j, "%c", 35);}//wall
      if(*array[i][j] == 0){mvprintw(i, j, "%c", 32);}//empty space
      if(*array[i][j] == 2){mvprintw(i, j, "%c", 64);}//player
      if(*array[i][j] == 3){mvprintw(i, j, "%c", 103);}//goblin
      if(*array[i][j] == 4){mvprintw(i, j, "=");}//water
      }

   }
}



int main()
{
   int i, j, k;
   int vara[MAX][MAX]; //going to implement int vara[MAX][MAX][CREATURES]; in order to store creature information
   int *ptra[MAX][MAX];
   //int creatures[MAX][MAX]; 
   int ch; // character buffer
   for ( i = 0; i < MAX; i++)
   {
      for( j = 0; j < MAX; j++)
      {
      ptra[i][j] = &vara[i][j]; //make pointer matrix point to data matrix
      }
   }
 
   testTwo(ptra);
   //printArray(ptra);
   mixer(ptra);
   placeWater(ptra);
   placeEnemies(ptra);
   //printArray(ptra);
   //printMap(ptra);
   i=1;
   j=1;
   system("clear");
   //initialize ncurses printing
   initscr();
   clear();
   noecho();
   cbreak();
   keypad(stdscr, true);
   //print a direction


   refresh();
   while(ch != 'q')
   {
    *ptra[i][j] = k; //replace ground under character with what was there
    ch = getch();

    //switch statement for controlling pointer (player) coordinates
    //uses collision detection by checking if wall or array boundary
    //is present where trying to go
    switch(ch)
    {
    case KEY_LEFT:
        if(*ptra[i][j-1]==3){*ptra[i][j-1]=0;} //kill if enemy in place
	if((j!=0)&&(*ptra[i][j-1]==0)){j=j-1;}
	break;
    case KEY_RIGHT:
        if(*ptra[i][j+1]==3){*ptra[i][j+1]=0;} //kill if enemy in place
	if((j<MAX-1)&&(*ptra[i][j+1]==0)){j=j+1;}
	break;
    case KEY_UP:
        if(*ptra[i-1][j]==3){*ptra[i-1][j]=0;} //kill if enemy in place
	if((i!=0)&&(*ptra[i-1][j]==0)){i=i-1;}
	break;
    case KEY_DOWN:
        if(*ptra[i+1][j]==3){*ptra[i+1][j]=0;} //kill if enemy in place
	if((i<MAX-1)&&(*ptra[i+1][j]==0)){i=i+1;}
	break;
    case 'd':
	//mine out in direction chosen via arrow keys
        mvprintw(0, 1, "SELECT DIRECTION TO MINE");
        ch=getch();
        switch(ch)
        {
        case KEY_LEFT:
            if((j!=0)&&(*ptra[i][j-1]==1)){*ptra[i][j-1]=0;}
            break;
        case KEY_RIGHT:
            if((j<MAX-1)&&(*ptra[i][j+1]==1)){*ptra[i][j+1]=0;}
            break;
        case KEY_UP:
            if((i!=0)&&(*ptra[i-1][j]==1)){*ptra[i-1][j]=0;}
            break;
        case KEY_DOWN:
            if((i<MAX-1)&&(*ptra[i+1][j]==1)){*ptra[i+1][j]=0;}
            break;
        }
        break;
    case 'b':
//build in direction chosen via arrow keys
        mvprintw(0, 1, "SELECT DIRECTION TO BUILD");
        ch=getch();
        switch(ch)
        {
        case KEY_LEFT:
            if((j!=0)&&(*ptra[i][j-1]==0)){*ptra[i][j-1]=1;}
            break;
        case KEY_RIGHT:
            if((j<MAX-1)&&(*ptra[i][j+1]==0)){*ptra[i][j+1]=1;}
            break;
        case KEY_UP:
            if((i!=0)&&(*ptra[i-1][j]==0)){*ptra[i-1][j]=1;}
            break;
        case KEY_DOWN:
            if((i<MAX-1)&&(*ptra[i+1][j]==0)){*ptra[i+1][j]=1;}
            break;
        }

    }
    //update map with location
    k = *ptra[i][j]; //store what will be under player in buffer 
    *ptra[i][j] = 2; //replace ground with character

    printMapc(ptra); //print the map
    //mvprintw(1,0, "%d", i);
    /*mvprintw(2, 0,
       ch == KEY_LEFT   ? "left"
      :ch == KEY_RIGHT  ? "right"
      :ch == KEY_UP     ? "up"
      :ch == KEY_DOWN   ? "down" : "");*/ //debug stuff
    clrtoeol();
    refresh();
   }
   endwin(); //end ncurses window
   return 0;
}
