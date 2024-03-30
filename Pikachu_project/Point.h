#ifndef POINT
#define POINT

#define _NORMAL 0
#define _LOCK 1
#define _DELETE -1

struct Point
{
	int coord_x = 0;					//coordinate x
	int coord_y = 0;					//coordinate y
	int current_state = 0;				//1: LOCK, 0: NORMAL, -1: DELETE
	char _pokemon = ' ';
	Point();							//constuctor

	int getX();							//get the coordinate x of current block
	int getY();							//get the coordinate y of current block
	int getCheck();						//get the current state of current block
	char getPokemons();					//get the pokemon of current block
	
	void setX(int);						//set the coordidate x to current block
	void setY(int);						//set the coordidate y to current block
	void setCheck(int);					//set the current state to current block
	void setPokemons(char);				//set the pokemon of current block

};

#endif