#include "Block.hpp"

/* constructor for Block;
 * bgt is the background on which the blocks will fall
 * texture contains the image representing each tile */
Block::Block(BGTiles * bgt, sf::Texture * texture)
{
	bposx =  bposy = 0;
	step = 0;
	currentRotation = 0;
	bgtiles = bgt;

	/* create and move sprites to "next block" area
	 * the exact location is set in subclass constructors */
	for (int i = 0; i < 4; i++)
		tiles[i].sprite = new sf::Sprite(*texture);

	/* choose the type of the block randomly */
	switch(rand() % 7)
	{
	/* line block:		****	*/
	case 0:
		for (int i = 0; i < 4; i++)
		{
			/* horizontal rotations */
			xRotations[0][i] = xRotations[2][i] = tiles[i].x = i;
			yRotations[0][i] = yRotations[2][i] = tiles[i].y = 1;

			/* vertical rotations */
			xRotations[1][i] = xRotations[3][i] = 1;
			yRotations[1][i] = yRotations[3][i] = i;

			tiles[i].sprite -> setColor(sf::Color::Red);
		}
		break;
	/* square block:	**
	 *					**		*/
	case 1:
		for (int i = 0; i < 4; i++)
		{
			xRotations[i][0] = tiles[0].x = 1;
			yRotations[i][0] = tiles[0].y = 1;

			xRotations[i][1] = tiles[1].x = 2;
			yRotations[i][1] = tiles[1].y = 1;


			xRotations[i][2] = tiles[2].x = 1;
			yRotations[i][2] = tiles[2].y = 2;

			xRotations[i][3] = tiles[3].x = 2;
			yRotations[i][3] = tiles[3].y = 2;

			tiles[i].sprite -> setColor(sf::Color::Yellow);
		}
		tiles[3].sprite -> setColor(sf::Color::Yellow);
		break;
	/* L1 block:		***
	 *					*		*/
	case 2:
		for (int i = 0; i < 3; i++)
		{
			xRotations[0][i] = tiles[i].x = i+1;
			yRotations[0][i] = tiles[i].y = 1;

			xRotations[1][i] = 2; yRotations[1][i] = i+1;
			xRotations[2][i] = i; yRotations[2][i] = 2;
			xRotations[3][i] = 1; yRotations[3][i] = i;

			tiles[i].sprite -> setColor(sf::Color::Green);
		}
		tiles[3].sprite -> setColor(sf::Color::Green);

		xRotations[0][3] = tiles[3].x = 1; 
		yRotations[0][3] = tiles[3].y = 2;
		
		xRotations[1][3] = 1; yRotations[1][3] = 1;
		xRotations[2][3] = 2; yRotations[2][3] = 1;
		xRotations[3][3] = 2; yRotations[3][3] = 2;
		
		break;
	/* L2 block			***
	 *					  *		*/
	case 3:
		for (int i = 0; i < 3; i++)
		{
			xRotations[0][i] = tiles[i].x = i;
			yRotations[0][i] = tiles[i].y = 1;

			xRotations[1][i] = 2;	yRotations[1][i] = i;
			xRotations[2][i] = i+1; yRotations[2][i] = 2;
			xRotations[3][i] = 1;	yRotations[3][i] = i+1;

			tiles[i].sprite -> setColor(sf::Color::Blue);
		}
		tiles[3].sprite -> setColor(sf::Color::Blue);

		xRotations[0][3] = tiles[3].x = 2;
		yRotations[0][3] = tiles[3].y = 2;

		xRotations[1][3] = 1; yRotations[1][3] = 2;
		xRotations[2][3] = 1; yRotations[2][3] = 1;
		xRotations[3][3] = 2; yRotations[3][3] = 1;

		break;
	/* S block			 **
	 *					**		*/
	case 4:
		for (int i = 0; i < 4; i++)
		{
			xRotations[i][0] = tiles[0].x = 0;
			yRotations[i][0] = tiles[0].y = 2;

			xRotations[i][1] = tiles[1].x = 1;
			yRotations[i][1] = tiles[1].y = 2;

			tiles[i].sprite -> setColor(sf::Color::Magenta);
		}
		xRotations[0][2] = xRotations[2][2] = tiles[2].x = 1; 
		yRotations[0][2] = yRotations[2][2] = tiles[2].y = 1;

		xRotations[1][2] = xRotations[3][2] = 0;
		yRotations[1][2] = yRotations[3][2] = 1;
		
		xRotations[0][3] = xRotations[2][3] = tiles[3].x = 2; 
		yRotations[0][3] = yRotations[2][3] = tiles[3].y = 1;

		xRotations[1][3] = xRotations[3][3] = 1;
		yRotations[1][3] = yRotations[3][3] = 3;
		
		break;
	/* Z block			**
	 *					 **		*/
	case 5:
		for (int i = 0; i < 4; i++)
		{
			xRotations[i][0] = tiles[0].x = 1;
			yRotations[i][0] = tiles[0].y = 2;

			xRotations[i][1] = tiles[1].x = 2;
			yRotations[i][1] = tiles[1].y = 2;

			tiles[i].sprite -> setColor(sf::Color::Cyan);
		}
		xRotations[0][2] = xRotations[2][2] = tiles[2].x = 0; 
		yRotations[0][2] = yRotations[2][2] = tiles[2].y = 1;

		xRotations[1][2] = xRotations[3][2] = 2;
		yRotations[1][2] = yRotations[3][2] = 1;
		
		xRotations[0][3] = xRotations[2][3] = tiles[3].x = 1; 
		yRotations[0][3] = yRotations[2][3] = tiles[3].y = 1;

		xRotations[1][3] = xRotations[3][3] = 1;
		yRotations[1][3] = yRotations[3][3] = 3;
		
		break;
	/* T block			 *
	 *					***		*/
	case 6:
		for (int i = 0; i < 3; i++)
		{
			xRotations[0][i] = xRotations[2][i] = tiles[i].x = i;
			yRotations[0][i] = yRotations[2][i] = tiles[i].y = 1;

			xRotations[1][i] = xRotations[3][i] = 1;	
			yRotations[1][i] = yRotations[3][i] = i;

			tiles[i].sprite -> setColor(sf::Color::White);
		}
		tiles[3].sprite -> setColor(sf::Color::White);

		xRotations[0][3] = tiles[3].x = 1;
		yRotations[0][3] = tiles[3].y = 2;

		xRotations[1][3] = 0; yRotations[1][3] = 1;
		xRotations[2][3] = 1; yRotations[2][3] = 0;
		xRotations[3][3] = 2; yRotations[3][3] = 1;
		break;
	}

	setPosition(NEXT_X, NEXT_Y);
}

/* checks if there is room in bgtiles for block, then adds it in
 * if there is not room, delete the offending tiles */
bool Block::makeActive()
{
	bool success = true;
	/* place at the top center of bgtiles */
	bposx = NUMCOLS / 2 - 2;
	bposy = 0;
	for (int i = 0; i < 4; i++)
		/* if tile does not fit, delete it and move on */
		if (bgtiles -> isTile(bposx+tiles[i].x,bposy+tiles[i].y))
		{
			success = false;
			delete tiles[i].sprite;
			tiles[i].sprite = 0;
			continue;
		}

	setPosition(BASEX, BASEY);

	/* if block does not fit, add tiles that do fit to 
	 * the background */
	if (!success)
		for (int i = 0; i < 4; i++)
			if (tiles[i].sprite)
			{
				tiles[i].x += bposx;
				tiles[i].y += bposy;
				bgtiles -> addTile(tiles[i]);
			}

	return success;
}

/* move all tiles down by a fraction of a square 
 * returns true if block was able to move */
bool Block::descend()
{
	int x, y;
	/* if there is no room to move, place tiles
	 * in the background and return false */
	if (step == 0)
		for (int i = 0; i < 4; i++)
		{
			x = bposx + tiles[i].x;
			y = bposy + tiles[i].y;

			if (y == NUMROWS - 1 || bgtiles -> isTile(x,y+1))
			{
				for (int j = 0; j < 4; j++)
				{
					tiles[j].x += bposx;
					tiles[j].y += bposy;
					bgtiles -> addTile(tiles[j]);
				}
				return false;
			}
		}

	/* otherwise move each tile down by a fraction of a square */
	if (++step == STEPS_PER_BLOCK)
	{
		step = 0;
		bposy++;
	}

	for (int i = 0; i < 4; i++)
		tiles[i].sprite -> move(sf::Vector2f(0,STEPSIZE));

	return true;
}

bool Block::descendBlock()
{
	int x, y;
	/* if there is no room to move, place tiles
	 * in the background and return false */
	for (int i = 0; i < 4; i++)
	{
		x = bposx + tiles[i].x;
		y = bposy + tiles[i].y;

		if (step == 0 && (y == NUMROWS - 1 || bgtiles -> isTile(x,y+1)))
		{
			for (int j = 0; j < 4; j++)
			{
				tiles[j].x += bposx;
				tiles[j].y += bposy;
				bgtiles -> addTile(tiles[j]);
			}
			return false;
		}
		/* if there is a fraction of a tile left */
		else if (step > 0 && (y >= NUMROWS - 2 || bgtiles -> isTile(x,y+2)))
		{
			step = 0;
			bposy++;
			setPosition(BASEX, BASEY);
			for (int j = 0; j < 4; j++)
			{
				tiles[j].x += bposx;
				tiles[j].y += bposy;
				bgtiles -> addTile(tiles[j]);
			}
			return false;

		}
	}

	/* otherwise move each tile down by a whole square */
	bposy++;

	for (int i = 0; i < 4; i++)
		tiles[i].sprite -> move(sf::Vector2f(0,TILESIZE));

	return true;
}


/* move all tiles one square to the left if possible */
void Block::moveLeft()
{
	int x, y;
	/* first check if there is room for all tiles one square to the left */
	for (int i = 0; i < 4; i++)
	{
		/* find coordinates of space to the left of current position */
		x = bposx + tiles[i].x - 1;
		y = bposy + tiles[i].y;

		/* if the tile would move outside the background or on an existing tile */
		if (x < 0 || 
			bgtiles -> isTile(x,y) ||
			(step > 0 && bgtiles -> isTile(x,y+1)))	//if descending, check next block down
		{
			return;
		}
	}

	/* if possible, move all 4 tiles */
	bposx--;
	for (int i = 0; i < 4; i++)
		tiles[i].sprite -> move(sf::Vector2f(-TILESIZE,0));
}

/* move all tiles one square to the right if possible */
void Block::moveRight()
{
	int x, y;
	/* first check if there is room for all tiles one square to the right */
	for (int i = 0; i < 4; i++)
	{
		x = bposx + tiles[i].x + 1;
		y = bposy + tiles[i].y;

		if (x >= NUMCOLS ||
			bgtiles -> isTile(x,y) ||
			(step > 0 && bgtiles -> isTile(x,y+1)))
		{
			return;
		}
	}
	/* if possible, move all 4 tiles */
	bposx++;
	for (int i = 0; i < 4; i++)
		tiles[i].sprite -> move(sf::Vector2f(TILESIZE,0));
}

void Block::draw(sf::RenderWindow & window)
{
	for (int i = 0; i < 4; i++)
		if (tiles[i].sprite)
			window.draw(*tiles[i].sprite);
}

void Block::rotate()
{
	int x, y;
	/* first check if the blocks can be rotated */
	for (int i = 0; i < 4; i++)
	{
		x = bposx + xRotations[(currentRotation + 1)%4][i];
		y = bposy + yRotations[(currentRotation + 1)%4][i];
		
		if (x >= NUMCOLS || x < 0 ||
			y >= NUMROWS || y < 0 ||
			bgtiles -> isTile(x,y) ||
			(step > 0 && bgtiles -> isTile(x,y+1)))
		{
			return;
		}
	}


	if (++currentRotation >= 4)
		currentRotation = 0;
	for (int i = 0; i < 4; i++)
	{
		tiles[i].x = xRotations[currentRotation][i];
		tiles[i].y = yRotations[currentRotation][i];
	}

	setPosition(BASEX, BASEY);
}

void Block::setPosition(int bx, int by)
{
	int x, y;
	for (int i = 0; i < 4; i++)
		if (tiles[i].sprite)
		{
			x = bx + (bposx + tiles[i].x)*TILESIZE;
			y = by + (bposy + tiles[i].y)*TILESIZE + step * STEPSIZE;
			tiles[i].sprite -> setPosition(x,y);
		}
}