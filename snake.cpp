#include <iostream>
#include <bits/stdc++.h>
#include <Windows.h>
#include <wchar.h>
#include <time.h>
using namespace std;

void delay(int milli_seconds) 
{  
    // Storing start time 
    clock_t start_time = clock(); 
  
    // looping till required time is not achieved 
      while (clock() < start_time + milli_seconds) 
        ; 
} 

int nScreenWidth = 80;			// Console Screen Size X (columns)
int nScreenHeight = 34;			// Console Screen Size Y (rows)
int high_score = 0;

struct snake_cell
{
	int x,y;
};

int main(){
	char *screen = new char[nScreenWidth*nScreenHeight];
	for (int i = 0; i < nScreenWidth*nScreenHeight; i++) 
		screen[i] = ' ';
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

//wscanf(L"%d",&level);


while(1){
	// Initialising the snake with 10 segments
	// First element is front
	list <snake_cell> snake = {{60,15} , {61,15} , {62,15} ,{63,15} ,{64,15} ,{65,15} ,{66,15} ,{67,15} ,{68,15} ,{69,15}  };
	int score=0;
	// Coordinates of food
	int foodX = 30;
	int foodY = 15;
	// Snake direction 0-3 is any of the four direction initially to left
	int snakeDirection = 3;
	bool dead=false;
	bool keyLeft = false, keyRight = false, keyLeftOld = false, keyRightOld = false,keyUp=false,keyDown=false;
	while(!dead)
	{

	// Timing and Input
		// This line is for the delay if not written snake will shoot off 
		// bcs CPU has very fast clock cycle , so this is written to slow down clock cycle
		//std::this_thread::sleep_for(200ms);
		//delay(100);
		// This thing work instead of delay 200ms because the delay 200ms gives the input of before 200 ms to the further code 
		// While this loop since input is calculated continously in 200ms time window so it gives
		// the most recent input to the rest of code
		// We are using two delays because change in aspect ratio of cmd window in x and y
		clock_t start_time = clock();
		while (clock() < start_time + ((snakeDirection%2==1)?100:180) )
		{
			// Get input
			keyRight = (0x8000 & GetAsyncKeyState((unsigned char)('\x27'))) != 0;
			keyLeft = (0x8000 & GetAsyncKeyState((unsigned char)('\x25'))) != 0;
			keyDown = (0x8000 & GetAsyncKeyState((unsigned char)('\x28'))) != 0;
			keyUp = (0x8000 & GetAsyncKeyState((unsigned char)('\x26'))) != 0;
			
			if(snakeDirection == 0)
			{
				if(keyLeft)
					snakeDirection=3;
				if(keyRight)
					snakeDirection=1;
			}
			if(snakeDirection == 1)
			{
				if(keyUp)
					snakeDirection=0;
				if(keyDown)
					snakeDirection=2;
			}	
			if(snakeDirection == 2)
			{
				if(keyLeft)
					snakeDirection=3;
				if(keyRight)
					snakeDirection=1;
			}
			if(snakeDirection == 3)
			{
				if(keyUp)
					snakeDirection=0;
				if(keyDown)
					snakeDirection=2;
			}
			/*if(keyRight && !keyRightOld)
			{
				snakeDirection++;
				if(snakeDirection==4)
					snakeDirection=0;
			}

			if(keyLeft && !keyLeftOld)
			{
				snakeDirection--;
				if(snakeDirection<0)
					snakeDirection=3;
			}
			keyLeftOld=keyLeft;
			keyRightOld=keyRight;*/
		}
	// Game Logic
		switch(snakeDirection)
		{
		case 0: // UP
			snake.push_front({snake.front().x , snake.front().y - 1});
			break;
		case 1: // Right
			snake.push_front({snake.front().x + 1,snake.front().y});
			break;
		case 2: // Down
			snake.push_front({snake.front().x , snake.front().y + 1});
			break;
		case 3: // Left
			snake.push_front({snake.front().x - 1,snake.front().y});
			break;
		}

		// Collision detection 
		// 1/3 - Collision with walls
		if(snake.front().x<0 || snake.front().x>=nScreenWidth)
			dead=true;
		if(snake.front().y<3 || snake.front().y>=nScreenHeight)
			dead=true;

		// 2/3 - Collision with food
		if(snake.front().x == foodX && snake.front().y == foodY)
		{
			score ++ ;
			// Now to make appear the next food location such that it does'nt
			// Coinide with snake's body
			while(screen[foodY * nScreenWidth + foodX] != ' ')
			{
				foodX = rand() % nScreenWidth;
				foodY = (rand() % (nScreenHeight - 3)) + 3 ;
			}

			// Growing snake by 2 units
			for(int i=0;i<5;i++)
				snake.push_back({snake.back().x,snake.back().y});
		} 

		// 3/3 - Collision detection with itself
		for(list<snake_cell> :: iterator i = snake.begin() ; i!= snake.end() ; i++)
			if(i!=snake.begin() && (*i).x == snake.front().x && (*i).y == snake.front().y)
				dead=true;

		// Chopping the snake's tail 
		snake.pop_back();

		
		

	// Display stuff to player
		// Clear screen
		for(int i=0 ; i<nScreenWidth*nScreenHeight ;i++)
			screen[i] = ' ';

		// Draw Game stats and borders
		for(int i=0;i<nScreenWidth;i++)
		{
			screen[i]='=';
			screen[2 * nScreenWidth + i]='=';
		}
		wsprintf(&screen[nScreenWidth + 5 ], "   Snake Game              Score : %d              High Score : %d ",score,high_score);

		//Draw snake body 
		for(auto s : snake)
			screen[s.y*nScreenWidth + s.x] = dead ? '+' : '0';
		//Draw snake head
			screen[snake.front().y * nScreenWidth + snake.front().x] = dead ? 'X' : '@';
		//Drawing the food
			screen[foodY * nScreenWidth + foodX] = '*';

		// if Dead
		if(dead)
		{
			high_score=max(high_score,score);
			wsprintf(&screen[14 * nScreenWidth + 14 ], "                 Your score :  %d                       ",score);
			wsprintf(&screen[15 * nScreenWidth + 14 ], "                    Snake DIED                          ");
			wsprintf(&screen[16 * nScreenWidth + 14 ], "             Press SPACE to PLAY again                  ");
		}

		// Display the frame
		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
	}

	// Wait for space 
	while((0x8000 & GetAsyncKeyState((unsigned char)('\x20'))) == 0);
}


	
return 0;
}
