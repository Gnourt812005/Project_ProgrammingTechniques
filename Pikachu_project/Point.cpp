#include "Point.h"
//constructor
Point::Point()
{
	coord_x = coord_y = current_state = 0;
}
// get the coordinate x of current block
int Point::getX()
{
	return coord_x;
}
// get the coordinate y of current block
int Point::getY()
{
	return coord_y;
}
// get the current state of current block
int Point::getCheck()
{
	return current_state;
}
//set the coordidate x to current block
void Point::setX(int x)
{
	coord_x = x;
}
//set the coordidate y to current block
void Point::setY(int y)
{
	coord_y = y;
}
//set the current state to current block
void Point::setCheck(int check)
{
	current_state = check;
}
//get the pokemon of current block
char Point::getPokemons()
{
	return _pokemon;
}
//set the pokemon of current block
void Point::setPokemons(char pokemon)
{
	_pokemon = pokemon;
}