#ifndef BLOCK
#define BLOCK


#include <SFML/Graphics.hpp>
#include "BGTiles.hpp"

struct Pos
{
	int x,y;
	Pos(int x,int y) : x(x), y(y) { }
	Pos(Pos & p) : x(p.x), y(p.y) { }

	Pos & operator=(const Pos & p) {x = p.x; y = p.y; return *this;}
	Pos & operator+(const Pos & p) { return Pos(x+p.x,y+p.y);}
	Pos & operator+=(const Pos & p){x += p.x; y += p.y; return *this;}
	Pos & operator*(int scale) { return Pos(x*scale,y*scale);}
};

struct Tile
{
	sf::Sprite * sprite; 
	int x, y;
};

#define STEPS_PER_BLOCK 32
#define TILESIZE	32
#define STEPSIZE	1
#define NEXT_X 540
#define NEXT_Y 144

class Block
{
	Tile tiles[4];
	int currentRotation, xRotations[4][4], yRotations[4][4];
	BGTiles * bgtiles;
	int bposx, bposy;
	int step;

public:
	Block(BGTiles * bgt, sf::Texture * texture);
	bool makeActive();
	bool descend();
	bool descendBlock();
	void moveLeft();
	void moveRight();
	void draw(sf::RenderWindow & window);
	void rotate();
	void setPosition(int bx, int by);
};

#endif