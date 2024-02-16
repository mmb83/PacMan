#include <iostream>
#include <cstdlib>
#include <conio.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>
#include <thread>
#include <windows.h>
#include <string>
using namespace std;
const int  boardLine = 11 , boardCol = 54 ;
int pacXpos =1 , pacYpos = 12 , score=0 ,ghost1Xpos = 5 , ghost1Ypos=1 ;
int ghost2Xpos = boardLine -2 , ghost2Ypos = boardCol -2 , foodNumbers=0 ,ghost3Xpos= boardLine-3 , ghost3Ypos = 25 ;
char  board[boardLine][boardCol]  , life = 3 , direct ;
int game = 3 , pacSpeed = 1, ghost1Speed = 2 , ghost2Speed = 2 ,ghost3Speed = 2 , level ,fakeX , fakeY;
bool specialFood=false , ghost1Eaten = false , ghost2Eaten = false , ghost3Eaten = false , ghostStop=false ;
time_t startTime , endTime ;
string menu[6];
void setCursorPosition(short int x ,short int y)
{
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = {x,y};
	SetConsoleCursorPosition(output , pos);
}
void setSpeed()
{
	if(level==1)
	{
		pacSpeed = 1, ghost1Speed = 2 , ghost2Speed = 2 ,ghost3Speed = 2 ;
	}
	if(level==2)
	{
		pacSpeed = 2, ghost1Speed = 2 , ghost2Speed = 2 , ghost3Speed = 2 ;
	}
	if(level==3)
	{
		pacSpeed=1 , ghost1Speed = 1 , ghost2Speed = 1 ,ghost3Speed = 1 ;
	}
}
int setSpecialFoods()
{
	if(level==1)
	return 50;
	if(level==2)
	return 30;
	if(level==3)
	return 20;
}
void setMenu()
{
	int position = 1 ;
	menu[0]="level :";
	menu[1]="Easy";
	menu[2]="Normal";
	menu[3]="Hard";
	menu[4]="life :";
	menu[5]="number";
	char key;
	while(key!=' ')
	{
	setCursorPosition(0,0);
	for(int i=0 ; i<4 ; i++){
		if(i==0){
				HANDLE h =GetStdHandle(STD_OUTPUT_HANDLE);
    			SetConsoleTextAttribute(h, 0x7);
		}
		else{
		
		if(i==position){
				HANDLE h =GetStdHandle(STD_OUTPUT_HANDLE);
    			SetConsoleTextAttribute(h, 0x6);
		}
		else{
			HANDLE h =GetStdHandle(STD_OUTPUT_HANDLE);
    		SetConsoleTextAttribute(h, 0x8);
		}
		}
	cout << menu[i] << endl;
	}
	key = getch();
	switch(key){
		case 'w' : case 'W':
		position--;
		if(position==0)
		position = 3;
		break;
		case 's' : case 'S':
		position++;
		if(position==4)
		position = 1;
		break;
	}
		level = position ;
	}
	key = 'q';
	while(key!=' ')
	{
		setCursorPosition(0,5);
		for(int i=4 ; i<6 ;i++)
		{
			if(i==4)
			{
				HANDLE h =GetStdHandle(STD_OUTPUT_HANDLE);
    			SetConsoleTextAttribute(h, 0x7);
			cout << menu[i] << endl;
			}
			else{
				HANDLE h =GetStdHandle(STD_OUTPUT_HANDLE);
    			SetConsoleTextAttribute(h, 0x8);
    			cout << menu[i]<<'<';
    			SetConsoleTextAttribute(h, 0x6);
				cout << game;
				SetConsoleTextAttribute(h, 0x8);
				cout <<'>' << endl;
    			
			}
			
		}
		key = getch();
		switch(key){
			case 'a' : case 'A':
				if(game>=2)
				game--;
				break;
			case 'd' : case 'D':
				if(game<=8)
				game++;
				break;
		}
	}
	setSpeed();
}
void foodCounter(int &foodNumbers)
{
	 for (int i = 0; i < boardLine; i++)
    {   	
        for (int j = 0; j < boardCol; j++)
        {
        	if(board[i][j]=='.'){
        		foodNumbers++;
			}
		}
	}
}
void playAgain()
{
	if(specialFood)
	{
		if(ghost1Xpos==pacXpos && ghost1Ypos==pacYpos){
			ghost1Xpos = 5 , ghost1Ypos=1 ;
			ghost1Eaten = true;
		}
		if(ghost2Xpos==pacXpos && ghost2Ypos==pacYpos){
			ghost2Xpos = 5 , ghost2Ypos= boardCol -2;
			ghost2Eaten = true;
		}
		if(ghost3Xpos==pacXpos && ghost3Ypos==pacYpos){
			ghost3Xpos=boardLine-3 , ghost3Ypos = 25;
			ghost3Eaten = true;
		}	
	}
	else
	{
	pacXpos =1 , pacYpos = 20 ;
	ghost1Xpos = 5 , ghost1Ypos=1 ;
	ghost2Xpos = 5 , ghost2Ypos= boardCol -2;
	ghost3Xpos=boardLine-3 , ghost3Ypos = 25;
	ghostStop=true;
	sleep(3);
	ghostStop=false;
	direct= 'w';
	}
}
bool loseGame()
{
	if((ghost1Xpos==pacXpos && ghost1Ypos==pacYpos)||(ghost2Xpos==pacXpos && ghost2Ypos==pacYpos)||(ghost3Xpos==pacXpos && ghost3Ypos==pacYpos))
	{
		if(specialFood)
		{
			if((ghost1Xpos==pacXpos && ghost1Ypos==pacYpos) && (ghost1Eaten==false)){
			score+=200;
			}
			if((ghost2Xpos==pacXpos && ghost2Ypos==pacYpos) && (ghost2Eaten==false)){
			score+=200;
			}
			if((ghost3Xpos==pacXpos && ghost3Ypos==pacYpos) && (ghost3Eaten==false)){
			score+=200;
			}
			playAgain();
		}
		else
		{
			return true;
		}
	}
	else
	{
		return false;
	}
}
bool winGame()
{
	if(foodNumbers==0){
		return true;
	}
	else{
		return false;
	}
}
void setBoard(int line ,int col,int boardLine , int boardCol)
{
	int seed;
    for (int i = 0; i < line; i++)
    {
        for (int j = 0; j < col ; j++)
        {
            	if (i ==0 || i == boardLine-1 || j == 0 || j == boardCol-1)
           		{
              	  board[i][j] = 'O';
                if(i==5 && (j==0 || j==boardCol-1 ))
                	board[i][j]=' ';
          	   	}
           			else
            		{
            		  	if (i == pacXpos && j == pacYpos )
          			  	{
               			 	board[i][j] = 'X';
            			}
            			else
            			{
            				if(((i>=1 && i<=boardLine-2) && (i!=3 && i!=boardLine-4) ) && (j==6||j==7 || j==boardCol-7 || j==boardCol-8 ))
            					board[i][j] = 'O';
							else if(((i>=2 && i<=4) ) && ((j>=2 && j<=4)||(j>=9 && j<=11) ||(j>=boardCol-12 && j<=boardCol-10)||(j>= boardCol-5 && j<= boardCol-3)))
									board[i][j] = 'O';
							else if((i==2 || i==4 || i==6 || i==8) && ((j>=13 && j<=23) || (j>=28 && j<=boardCol - 14)))
									board[i][j]='O';
							else if((i<= boardLine-3 && i>= boardLine-5) && ((j>=2 && j<=4)||(j>=9 && j<=11)||(j>= boardCol-5 && j<= boardCol-3)||(j>=boardCol-12 && j<=boardCol-10)))
								board[i][j] = 'O';
							else if(((i>=1 && i<=boardLine-2)&&(i!=2 && i!=boardLine-3))&&(j==25 || j==26))	
								board[i][j]='O';
						/*	else if((i==1 || i ==2 || i==boardLine-2 || i==boardLine-3 ) & (j==boardCol-7 || j==boardCol-8))
									board[i][j] = 'O';
							else if((i==2 || i==4) && (j>=28 && j<=boardCol - 14))
									board[i][j]='O';
							else if((i== boardLine-3 || i== boardLine-5) && (j>=13 && j<=23))
									board[i][j] = 'O';
							else if((i== boardLine-3 || i== boardLine-5) && (j>=28 && j<=boardCol - 14))	
									board[i][j]='O';
							else if((i>=2 && i<=4) && (j>=2 && j<=4))
									board[i][j] = 'O';
							else if((i>=2 && i<=4) && (j>=boardCol-12 && j<=boardCol-10))
									board[i][j] = 'O';
							else if((i>=4 && i<=6)&&(j>= boardCol-8 && j<= boardCol-7))
									board[i][j] = 'O';
							else if((i>=4 && i<=6)&&(j>=6 && j<=7)) 
									board[i][j] = 'O';	
							else if((i==1 || i== boardLine-2)&&(j==25 || j==26))
									board[i][j]='O';
							else if((i>=boardLine-5 && i<=boardLine-3) && (j>=9 && j<=11))
									board[i][j] = 'O';
							else if((i<= boardLine-3 && i>= boardLine-5) && (j>=2 && j<=4))
									board[i][j] = 'O';
							else if((i>=2 && i<=4) && (j>= boardCol-5 && j<= boardCol-3) )
									board[i][j] = 'O';
							else if((i<= boardLine-3 && i>= boardLine-5) && (j>= boardCol-5 && j<= boardCol-3) )
									board[i][j] = 'O';*/
							else 
							{
								int chance;
								chance=setSpecialFoods();
								seed++;
								srand(seed);
								if(rand()%chance==i){
									board[i][j] = 15;
								}
								else{
									board[i][j] = '.';
								}
									
							}
						}
           			}
        }
    }
    foodCounter(foodNumbers);
}
void setRandomBoard()
{
	int chance , mode , difference;
	int seed;
	for(int i = 0 ;i < boardLine ;i++)
	{
		for(int j = 0 ;j<boardCol ;j++)
		{
				if (i ==0 || i == boardLine-1 || j == 0 || j == boardCol-1)
           		{
              	  board[i][j] = 'O';
              	  if(i==5 && (j==0 || j==boardCol-1 ))
                	board[i][j]=' ';
				}
					else 
					{
						
						srand(seed);
						//chance=rand()%5+2;
						//mode=rand()%5;
						//difference=rand()%3+1;
						if(i==1 || i == boardLine - 2 ||j==1 || j==boardCol - 2)
						{
							board[i][j]='.';
						}
						else
						{
							if (i == pacXpos && j == pacYpos )
          			  		{
               			 		board[i][j] = 'X';
            				}
            				else 
							{
							if(rand()%6==0)
								board[i][j]='O';
							else
							{
								seed++;
								srand(seed);
								if(rand()%50==j){
									board[i][j] = 15;
								}
								else{
									board[i][j] = '.';
								}		
							}
						}
					}	
						seed+=1;
				}
			}
		}
}
void setBoard(int line ,int col)
{
	for(int i = 0 ; i < line ; i++ ) 
	{
		for(int j = 0 ; j < col ; j++)
		{
			if(board[i][j]=='X')
			{
				board[i][j] = ' ';
			}
			if(i == pacXpos && j == pacYpos )
			{
				if(board[i][j] == '.'){
          			score+=10;
          			foodNumbers--;
				}
				if(board[i][j]==15)
				{
					time(&startTime);
					specialFood=true;
					pacSpeed=1;
					ghost1Speed = 3 , ghost2Speed = 3 ,ghost3Speed = 3;
				}	
				else if(int(endTime - startTime)==10)
				{
					ghost1Eaten == false , ghost2Eaten == false , ghost3Eaten == false;
					specialFood=false;
					setSpeed();
				}
				time(&endTime);
				board[i][j] = 'X';
			}
		}
	}
}
void printBoard(int boardLine, int boardCol)
{
	setCursorPosition(0,1);
    for (int i = 0; i < boardLine; i++)
    {   	
        for (int j = 0; j < boardCol; j++)
        {
        	 if(i == ghost1Xpos && j == ghost1Ypos)
			{
				if(specialFood == false ){
				HANDLE h =GetStdHandle(STD_OUTPUT_HANDLE);
    			SetConsoleTextAttribute(h, 0x4);
				}
    			else {
    			HANDLE h =GetStdHandle(STD_OUTPUT_HANDLE);
    			SetConsoleTextAttribute(h, 11);
				}
    			cout << 'Q';
    			continue;
			}
			if(i == ghost2Xpos && j == ghost2Ypos)
			{	
				if (specialFood==false ){
				HANDLE h =GetStdHandle(STD_OUTPUT_HANDLE);
    			SetConsoleTextAttribute(h, 0x8);
				}
    			else {
    			HANDLE h =GetStdHandle(STD_OUTPUT_HANDLE);
    			SetConsoleTextAttribute(h, 11);
				}
    			cout << '@';
    			continue;
			}
			if(i == ghost3Xpos && j == ghost3Ypos)
			{
				if(specialFood==false){
				HANDLE h =GetStdHandle(STD_OUTPUT_HANDLE);
    			SetConsoleTextAttribute(h, 0x6);
				}
    			else {
    			HANDLE h =GetStdHandle(STD_OUTPUT_HANDLE);
    			SetConsoleTextAttribute(h, 11);
				}
    			cout << '$';
    			continue;
			}
        	else if(i==pacXpos && j==pacYpos){
        		HANDLE h =GetStdHandle(STD_OUTPUT_HANDLE);
    			SetConsoleTextAttribute(h, 0x6);
    			cout << 'X';
    			continue;
			}
			else if(board[i][j]=='O')
			{
				HANDLE h =GetStdHandle(STD_OUTPUT_HANDLE);
    			SetConsoleTextAttribute(h, 0x11); //blue 11 ,green 22 ,
			}
			else if(board[i][j]=='.')
			{
				HANDLE h =GetStdHandle(STD_OUTPUT_HANDLE);
    			SetConsoleTextAttribute(h, 10);
			}
			else if(board[i][j]==15)
			{
				HANDLE h =GetStdHandle(STD_OUTPUT_HANDLE);
    			SetConsoleTextAttribute(h, 13);
			}
			else
			{
				HANDLE h =GetStdHandle(STD_OUTPUT_HANDLE);
    			SetConsoleTextAttribute(h, 0x7);
			}
            	cout << board[i][j];
        }
        cout << endl ;
        
    }
    if(loseGame())
	{
		if(specialFood==false){
		game--;
		}
		if(game>=0)
		playAgain();
		printBoard(boardLine , boardCol);
		if(game<=0)
		{
		setCursorPosition(21,12);
		HANDLE h =GetStdHandle(STD_OUTPUT_HANDLE);
    	SetConsoleTextAttribute(h, 0x4);
		cout << "game Over!!";
		}
	}
	else if(winGame()){
		setCursorPosition(12,12);
		HANDLE h =GetStdHandle(STD_OUTPUT_HANDLE);
    	SetConsoleTextAttribute(h, 0x2);
    	cout << "Winner Winner Chicken Dinner!!";
    	game=0;
	}
	else {
		setCursorPosition(0,0);
		HANDLE h =GetStdHandle(STD_OUTPUT_HANDLE);
    	SetConsoleTextAttribute(h, 0x7);
    	cout << "Score: " << score ;
	}	
	setCursorPosition(1,12);
	HANDLE h =GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h, 0x4);
    cout << life;
	setCursorPosition(0,12);
	cout << game;
}
void pacMove(int& pacXpos , int& pacYpos)
{
	
	while(game>0)
	{
		if(kbhit()){
		direct = getch();
		} 
	    switch(direct) 
	    {
		case 'a' : case 'A':      
		if( board[pacXpos][pacYpos-1] !='O' )
		pacYpos--;	
		if(pacYpos==0)
		pacYpos=boardCol -1;
		break;	
		case 'd' : case 'D':
		if( board[pacXpos][pacYpos+1] !='O' )
		pacYpos++;	
		if(pacYpos==boardCol-1)
		pacYpos=0;
		break;
		case 'w' : case 'W':
		if( board[pacXpos-1][pacYpos] !='O')
		pacXpos--;	
		break;
		case 's' : case 'S':
		if( board[pacXpos+1][pacYpos] !='O')
		pacXpos++;	
		break;
		default :
		break;
		}
		setBoard(boardLine,boardCol);
    	printBoard(boardLine , boardCol);
		usleep(pacSpeed * 100000);
		if(loseGame())
		{
			setBoard(boardLine,boardCol);
			printBoard(boardLine , boardCol);
		}
	}
}
void ghost1Move(int& ghost1Xpos , int& ghost1Ypos)
{
	int seed = 0;
	int xDistance,yDistance ;
	char a;
	while(game>0)
	{
	if( ghostStop){
		
	}
	else
	{
	seed ++;
	srand(seed);
	if(rand()%4==0 && level!=3){
		usleep(2*100000);
	}
	//a = 's';
	else
	{
	if((specialFood) && ((ghost1Ypos - pacYpos)<=25 && (ghost1Ypos - pacYpos)>=-25) ){
	xDistance = pacXpos - ghost1Xpos ;
	yDistance = pacYpos - ghost1Ypos ;
	}
	else {
	xDistance = ghost1Xpos - pacXpos ;
	yDistance = ghost1Ypos - pacYpos ;
	}
	if(xDistance>=-2 && yDistance>0 && board[ghost1Xpos][ghost1Ypos-1] !='O' )
	a = 'a';
	else if(xDistance>=-2 && yDistance<0 && board[ghost1Xpos][ghost1Ypos+1] !='O')
	a = 'd';
	else if(xDistance>0 && yDistance<=0 && board[ghost1Xpos-1][ghost1Ypos] !='O')
	a = 'w';
	else if(xDistance<=-2 && yDistance>=0 && board[ghost1Xpos+1][ghost1Ypos] !='O')
	a = 's';
	}
	    switch(a) 
	    {
		case 'a' : case 'A':      
		if( board[ghost1Xpos][ghost1Ypos-1] !='O' )
		ghost1Ypos--;
		else if(board[ghost1Xpos][ghost1Ypos-1] =='O' && board[ghost1Xpos-1][ghost1Ypos] =='O')	
		a = 's';
		else if(board[ghost1Xpos][ghost1Ypos-1] =='O' && board[ghost1Xpos+1][ghost1Ypos] =='O')	
		a = 'w';
		else if(board[ghost1Xpos][ghost1Ypos-1] =='O')
		a = 'w';
		if(ghost1Ypos==0)
		ghost1Ypos=boardCol -1;
		break;	
		case 'd' : case 'D':
		if( board[ghost1Xpos][ghost1Ypos+1] !='O' )
		ghost1Ypos++;	
		else if(ghost1Ypos==boardCol-1)
		ghost1Ypos=0;
		else if(board[ghost1Xpos][ghost1Ypos+1] =='O' && board[ghost1Xpos-1][ghost1Ypos] =='O' )	
		a = 's';
		else if(board[ghost1Xpos][ghost1Ypos+1] =='O' && board[ghost1Xpos+1][ghost1Ypos] =='O')	
		a = 'w';
		else if(board[ghost1Xpos][ghost1Ypos+1] =='O')
		a = 'w';
		break;
		case 'w' : case 'W':
		if( board[ghost1Xpos-1][ghost1Ypos] !='O')
		ghost1Xpos--;
		else if(board[ghost1Xpos-1][ghost1Ypos] =='O' && board[ghost1Xpos][ghost1Ypos+1] =='O' )	
		a = 'a';	
		else if(board[ghost1Xpos-1][ghost1Ypos] =='O' && board[ghost1Xpos][ghost1Ypos-1] =='O' )	
		a = 'd';
		else if(board[ghost1Xpos-1][ghost1Ypos] =='O')
		a = 'd';
		break;
		case 's' : case 'S':
		if( board[ghost1Xpos+1][ghost1Ypos] !='O')	
		ghost1Xpos++;
		else if(board[ghost1Xpos+1][ghost1Ypos] =='O' && board[ghost1Xpos][ghost1Ypos+1] =='O')	
		a = 'a';
		else if(board[ghost1Xpos+1][ghost1Ypos] =='O' && board[ghost1Xpos][ghost1Ypos-1] =='O')	
		a = 'd';
		else if(board[ghost1Xpos+1][ghost1Ypos] =='O')
		a = 'a';
		break;
		default :
		break;
	}
		setBoard(boardLine,boardCol);
		usleep(ghost1Speed * 100000);
    		if(loseGame() )
			{
			setBoard(boardLine,boardCol);
			} 
		}
	}
}
void ghost2Move(int& ghost2Xpos , int& ghost2Ypos)
{
	int seed=0;
	int xDistance,yDistance ;
	char a;
	//a = 'w';
	while(game>0)
	{
	if( ghostStop){

	}
	else
	{
	if((ghost2Xpos==ghost1Xpos && ghost2Ypos==ghost1Ypos) )
	{
		usleep(9*100000);
	}
	seed ++;
	srand(seed);
	if(rand()%5==0 && level!=3)
	{
		usleep(3*100000);
	}
	else 
	{
	if((specialFood) && ((ghost2Ypos - pacYpos)<=25 && (ghost2Ypos - pacYpos)>=-25) ){
	xDistance = pacXpos - ghost2Xpos ;
	yDistance = pacYpos - ghost2Ypos ;
	}
	else
	{
	xDistance = ghost2Xpos - pacXpos ;
	yDistance = ghost2Ypos - pacYpos ;
	}
	if(xDistance>=-2 && yDistance>0 && board[ghost2Xpos][ghost2Ypos-1] !='O' )
	a = 'a';
	else if(xDistance>=0 && yDistance<0 && board[ghost2Xpos][ghost2Ypos+1] !='O')
	a = 'd';
	else if(xDistance>1 && yDistance<=0 && board[ghost2Xpos-1][ghost2Ypos] !='O')
	a = 'w';
	else if(xDistance<-1 && yDistance>=0 && board[ghost2Xpos+1][ghost2Ypos] !='O')
	a = 's';
	}
	    switch(a) 
	    {
		case 'a' : case 'A':      
		if( board[ghost2Xpos][ghost2Ypos-1] !='O' )
		ghost2Ypos--;
		else if(board[ghost2Xpos][ghost2Ypos-1] =='O' && board[ghost2Xpos-1][ghost2Ypos] =='O')	
		a = 's';
		else if(board[ghost2Xpos][ghost2Ypos-1] =='O' || board[ghost2Xpos+1][ghost2Ypos] =='O')	
		a = 'w';
		else if(board[ghost2Xpos][ghost2Ypos-1] =='O')
		a = 'w';
		if(ghost2Ypos==0)
		ghost2Ypos=boardCol -1;
		break;	
		case 'd' : case 'D':
		if( board[ghost2Xpos][ghost2Ypos+1] !='O' )
		ghost2Ypos++;	
		else if(ghost2Ypos==boardCol-1)
		ghost2Ypos=0;
		else if(board[ghost2Xpos][ghost2Ypos+1] =='O' && board[ghost2Xpos-1][ghost2Ypos] =='O' )	
		a = 's';
		else if(board[ghost2Xpos][ghost2Ypos+1] =='O' && board[ghost2Xpos+1][ghost2Ypos] =='O')	
		a = 'w';
		else if(board[ghost2Xpos][ghost2Ypos+1] =='O')
		a = 'w';
		break;
		case 'w' : case 'W':
		if( board[ghost2Xpos-1][ghost2Ypos] !='O')
		ghost2Xpos--;
		else if(board[ghost2Xpos-1][ghost2Ypos] =='O' && board[ghost2Xpos][ghost2Ypos+1] =='O' )	
		a = 'a';	
		else if(board[ghost2Xpos-1][ghost2Ypos] =='O' && board[ghost2Xpos][ghost2Ypos-1] =='O' )	
		a = 'd';
		else if(board[ghost2Xpos-1][ghost2Ypos] =='O')
		a = 'd';
		break;
		case 's' : case 'S':
		if( board[ghost2Xpos+1][ghost2Ypos] !='O')	
		ghost2Xpos++;
		else if(board[ghost2Xpos+1][ghost2Ypos] =='O' && board[ghost2Xpos][ghost2Ypos+1] =='O')	
		a = 'a';
		else if(board[ghost2Xpos+1][ghost2Ypos] =='O' && board[ghost2Xpos][ghost2Ypos-1] =='O')	
		a = 'd';
		else if(board[ghost2Xpos+1][ghost2Ypos] =='O')
		a = 'a';
		break;
		default :
		break;
	}
  	usleep(ghost2Speed * 100000);
		}
	}
}
void ghost3Move(int& ghost3Xpos,int& ghost3Ypos)
{
	int seed=0;
	int xDistance,yDistance ;
	char a ='a';
	while(game>0)
	{
	if( ghostStop){

	}
	else
	{
	if((ghost3Xpos==ghost1Xpos && ghost3Ypos==ghost1Ypos) || (ghost3Xpos==ghost2Xpos && ghost3Ypos==ghost2Ypos) )
	{
		usleep(9*100000);
	}
	if(rand()%6==0 && level!=3)
	{
		usleep(2*100000);
	}
	seed+=1;
	srand(seed);
	fakeX=rand()%10+1;
	fakeY=rand()%50+1;
	if(board[fakeX][fakeY]=='O')
	{
		fakeX=rand()%9+1;
		fakeY=rand()%52+1;
	}
	if(specialFood)
	{
	if( (((ghost3Ypos - pacYpos)<=25) && (ghost3Ypos - pacYpos)>=-25) ){
	xDistance = pacXpos - ghost3Xpos ;
	yDistance = pacYpos - ghost3Ypos ;
	}
	else
	{
		xDistance = ghost3Xpos - pacXpos ;
		yDistance = ghost3Ypos - pacYpos ;
	}
}
	else{
	
	if( (((ghost3Xpos - pacXpos)>=-5) || ((ghost3Xpos - pacXpos)<=5)) && (((ghost3Ypos - pacYpos)>=-20) || ((ghost3Ypos - pacYpos)<=20)))
	{
		xDistance = ghost3Xpos - pacXpos ;
		yDistance = ghost3Ypos - pacYpos ;
	}
	else
	{
		xDistance = ghost3Xpos - fakeX ;
		yDistance = ghost3Ypos - fakeY ;
		fakeX=rand()%10+1;
		fakeY=rand()%50;
	}
}
	if(xDistance>=-2 && yDistance>0 && board[ghost3Xpos][ghost3Ypos-1] !='O' )
	a = 'a';
	else if(xDistance>=0 && yDistance<0 && board[ghost3Xpos][ghost3Ypos+1] !='O')
	a = 'd';
	else if(xDistance>1 && yDistance<=0 && board[ghost3Xpos-1][ghost3Ypos] !='O')
	a = 'w';
	else if(xDistance<-1 && yDistance>=0 && board[ghost3Xpos+1][ghost3Ypos] !='O')
	a = 's';
	switch(a)
	{
	case 'a' : case 'A':      
		if( board[ghost3Xpos][ghost3Ypos-1] !='O' )
		ghost3Ypos--;
		else if(board[ghost3Xpos][ghost3Ypos-1] =='O' && board[ghost3Xpos-1][ghost3Ypos] =='O')	
		a = 's';
		else if(board[ghost3Xpos][ghost3Ypos-1] =='O' || board[ghost3Xpos+1][ghost3Ypos] =='O')	
		a = 'w';
		else if(board[ghost3Xpos][ghost3Ypos-1] =='O')
		a = 'w';
		if(ghost3Ypos==0)
		ghost3Ypos=boardCol -1;
		break;	
		case 'd' : case 'D':
		if( board[ghost3Xpos][ghost3Ypos+1] !='O' )
		ghost3Ypos++;	
		else if(ghost3Ypos==boardCol-1)
		ghost3Ypos=0;
		else if(board[ghost3Xpos][ghost3Ypos+1] =='O' && board[ghost3Xpos-1][ghost3Ypos] =='O' )	
		a = 's';
		else if(board[ghost3Xpos][ghost3Ypos+1] =='O' && board[ghost3Xpos+1][ghost3Ypos] =='O')	
		a = 'w';
		else if(board[ghost3Xpos][ghost3Ypos+1] =='O')
		a = 'w';
		break;
		case 'w' : case 'W':
		if( board[ghost3Xpos-1][ghost3Ypos] !='O')
		ghost3Xpos--;
		else if(board[ghost3Xpos-1][ghost3Ypos] =='O' && board[ghost3Xpos][ghost3Ypos+1] =='O' )	
		a = 'a';	
		else if(board[ghost3Xpos-1][ghost3Ypos] =='O' && board[ghost3Xpos][ghost3Ypos-1] =='O' )	
		a = 'd';
		else if(board[ghost3Xpos-1][ghost3Ypos] =='O')
		a = 'd';
		break;
		case 's' : case 'S':
		if( board[ghost3Xpos+1][ghost3Ypos] !='O')	
		ghost3Xpos++;
		else if(board[ghost3Xpos+1][ghost3Ypos] =='O' && board[ghost3Xpos][ghost3Ypos+1] =='O')	
		a = 'a';
		else if(board[ghost3Xpos+1][ghost3Ypos] =='O' && board[ghost3Xpos][ghost3Ypos-1] =='O')	
		a = 'd';
		else if(board[ghost3Xpos+1][ghost3Ypos] =='O')
		a = 'a';
		break;
		default :
		break;
	}
	if(loseGame() ){
	setBoard(boardLine,boardCol);
	}
	usleep(ghost3Speed * 100000);
		}
	}
}
int main()
{  
		setMenu();
		setBoard(boardLine,boardCol,boardLine,boardCol);
	//	setRandomBoard();
		thread t1 =thread(pacMove,ref(pacXpos),ref(pacYpos));
		thread t2 =thread(ghost1Move,ref(ghost1Xpos),ref(ghost1Ypos));
		thread t3 =thread(ghost2Move,ref(ghost2Xpos),ref(ghost2Ypos));
		thread t4 =thread(ghost3Move,ref(ghost3Xpos),ref(ghost3Ypos));
		t1.join();
		t2.join();
		t3.join();
		t4.join();
}
