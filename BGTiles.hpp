#ifndef BGTILES
#define BGTILES


#include <SFML/Graphics.hpp>

#define NUMROWS	18
#define NUMCOLS	10
#define BASEX 24
#define BASEY 0


struct Tile;

class BGTiles
{
	(sf::Sprite *) sprites[NUMROWS][NUMCOLS];
	int rowSum[NUMROWS];
	int step;

public:
	BGTiles();
	~BGTiles();
	bool addTile(Tile t);
	bool checkRows();
	int removeRows();
	bool descend();
	void blink();
	void draw(sf::RenderWindow & window);
	bool isTile(int x, int y);
	void reset();
};

#endif