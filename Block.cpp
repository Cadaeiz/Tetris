#include "Block.hpp"

/* constructor for Block;
 * bgt is the background on which the blocks will fall
 * texture contains the image representing each tile */
Block::Block(BGTiles * bgt, sf::Texture * texture)
{
	bpos = Pos();
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
			rotations[0][i] = rotations[2][i] = tiles[i].pos = Pos(i,1);
			rotations[1][i] = rotations[3][i] = Pos(1,i);

			tiles[i].sprite -> setColor(sf::Color::Red);
		}
		break;
	/* square block:	**
	 *					**		*/
	case 1:
		for (int i = 0; i < 4; i++)
		{
			rotations[i][0] = tiles[0].pos = Pos(1,1);
			rotations[i][1] = tiles[1].pos = Pos(2,1);
			rotations[i][2] = tiles[2].pos = Pos(1,2);
			rotations[i][3] = tiles[3].pos = Pos(2,2);

			tiles[i].sprite -> setColor(sf::Color::Yellow);
		}
		break;
	/* L1 block:		***
	 *					*		*/
	case 2:
		for (int i = 0; i < 3; i++)
		{
			rotations[0][i] = tiles[i].pos = Pos(i+1,1);
			rotations[1][i] = Pos(2,i+1);
			rotations[2][i] = Pos(i,2);
			rotations[3][i] = Pos(1,i);

			tiles[i].sprite -> setColor(sf::Color::Green);
		}
		tiles[3].sprite -> setColor(sf::Color::Green);

		rotations[0][3] = tiles[3].pos = Pos(1,2);
		rotations[1][3] = Pos(1,1);
		rotations[2][3] = Pos(2,1);
		rotations[3][3] = Pos(2,2);

		break;
	/* L2 block			***
	 *					  *		*/
	case 3:
		for (int i = 0; i < 3; i++)
		{
			rotations[0][i] = tiles[i].pos = Pos(i,1);
			rotations[1][i] = Pos(2,i);
			rotations[2][i] = Pos(i+1,2);
			rotations[3][i] = Pos(1,i+1);

			tiles[i].sprite -> setColor(sf::Color::Blue);
		}
		tiles[3].sprite -> setColor(sf::Color::Blue);

		rotations[0][3] = tiles[3].pos = Pos(2,2);
		rotations[1][3] = Pos(1,2);
		rotations[2][3] = Pos(1,1);
		rotations[3][3] = Pos(2,1);

		break;
	/* S block			 **
	 *					**		*/
	case 4:
		for (int i = 0; i < 4; i++)
		{
			rotations[i][0] = tiles[0].pos = Pos(0,2);
			rotations[i][1] = tiles[1].pos = Pos(1,2);

			tiles[i].sprite -> setColor(sf::Color::Magenta);
		}
		rotations[0][2] = rotations[2][2] = tiles[2].pos = Pos(1,1);
		rotations[1][2] = rotations[3][2] = Pos(0,1);
		rotations[0][3] = rotations[2][3] = tiles[3].pos = Pos(2,1);
		rotations[1][3] = rotations[3][3] = Pos(1,3);

		break;
	/* Z block			**
	 *					 **		*/
	case 5:
		for (int i = 0; i < 4; i++)
		{
			rotations[i][0] = tiles[0].pos = Pos(1,2);
			rotations[i][1] = tiles[1].pos = Pos(2,2);

			tiles[i].sprite -> setColor(sf::Color::Cyan);
		}
		rotations[0][2] = rotations[2][2] = tiles[2].pos = Pos(0,1);
		rotations[1][2] = rotations[3][2] = Pos(2,1);
		rotations[0][3] = rotations[2][3] = tiles[3].pos = Pos(1,1);
		rotations[1][3] = rotations[3][3] = Pos(1,3);
		
		break;
	/* T block			 *
	 *					***		*/
	case 6:
		for (int i = 0; i < 3; i++)
		{
			rotations[0][i] = rotations[2][i] = tiles[i].pos = Pos(i,1);
			rotations[1][i] = rotations[3][i] = Pos(1,i);

			tiles[i].sprite -> setColor(sf::Color::White);
		}
		tiles[3].sprite -> setColor(sf::Color::White);

		rotations[0][3] = tiles[3].pos = Pos(1,2);
		rotations[1][3] = Pos(0,1);
		rotations[2][3] = Pos(1,0);
		rotations[3][3] = Pos(2,1);

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
	bpos = Pos(NUMCOLS / 2 - 2, 0);

	for (int i = 0; i < 4; i++)
		/* if tile does not fit, delete it and move on */
		if (bgtiles -> isTile(bpos+tiles[i].pos))
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
				tiles[i].pos += bpos;
				bgtiles -> addTile(tiles[i]);
			}

	return success;
}

/* move all tiles down by a fraction of a square 
 * returns true if block was able to move */
bool Block::descend()
{
	Pos p;
	/* if there is no room to move, place tiles
	 * in the background and return false */
	if (step == 0)
		for (int i = 0; i < 4; i++)
		{
			p = bpos + tiles[i].pos;

			if (p.y == NUMROWS - 1 || bgtiles -> isTile(p+Pos(0,1)))
			{
				for (int j = 0; j < 4; j++)
				{
					tiles[j].pos += bpos;
					bgtiles -> addTile(tiles[j]);
				}
				return false;
			}
		}

	/* otherwise move each tile down by a fraction of a square */
	if (++step == STEPS_PER_BLOCK)
	{
		step = 0;
		bpos.y++;
	}

	for (int i = 0; i < 4; i++)
		tiles[i].sprite -> move(0,STEPSIZE);

	return true;
}

bool Block::descendBlock()
{
	Pos p;
	/* if there is no room to move, place tiles
	 * in the background and return false */
	for (int i = 0; i < 4; i++)
	{
		p = bpos + tiles[i].pos;

		if (step == 0 && (p.y == NUMROWS - 1 || bgtiles -> isTile(p+Pos(0,1))))
		{
			for (int j = 0; j < 4; j++)
			{
				tiles[j].pos += bpos;
				bgtiles -> addTile(tiles[j]);
			}
			return false;
		}
		/* if there is a fraction of a tile left */
		else if (step > 0 && (p.y >= NUMROWS - 2 || bgtiles -> isTile(p+Pos(0,2))))
		{
			step = 0;
			bpos.y++;
			setPosition(BASEX, BASEY);
			for (int j = 0; j < 4; j++)
			{
				tiles[j].pos += bpos;
				bgtiles -> addTile(tiles[j]);
			}
			return false;

		}
	}

	/* otherwise move each tile down by a whole square */
	bpos.y++;

	for (int i = 0; i < 4; i++)
		tiles[i].sprite -> move(0,TILESIZE);

	return true;
}


/* move all tiles one square to the left if possible */
void Block::moveLeft()
{
	Pos p;
	/* first check if there is room for all tiles one square to the left */
	for (int i = 0; i < 4; i++)
	{
		/* find coordinates of space to the left of current position */
		p = bpos + tiles[i].pos + Pos(-1,0);

		/* if the tile would move outside the background or on an existing tile */
		if (p.x < 0 || 
			bgtiles -> isTile(p) ||
			(step > 0 && bgtiles -> isTile(p+Pos(0,1))))	//if descending, check next block down
		{
			return;
		}
	}

	/* if possible, move all 4 tiles */
	bpos.x--;
	for (int i = 0; i < 4; i++)
		tiles[i].sprite -> move(-TILESIZE,0);
}

/* move all tiles one square to the right if possible */
void Block::moveRight()
{
	Pos p;
	/* first check if there is room for all tiles one square to the right */
	for (int i = 0; i < 4; i++)
	{
		p = bpos + tiles[i].pos + Pos(1,0);

		if (p.x >= NUMCOLS ||
			bgtiles -> isTile(p) ||
			(step > 0 && bgtiles -> isTile(p+Pos(0,1))))
		{
			return;
		}
	}
	/* if possible, move all 4 tiles */
	bpos.x++;
	for (int i = 0; i < 4; i++)
		tiles[i].sprite -> move(TILESIZE,0);
}

void Block::draw(sf::RenderWindow & window)
{
	for (int i = 0; i < 4; i++)
		if (tiles[i].sprite)
			window.draw(*tiles[i].sprite);
}

void Block::rotate()
{
	Pos p;
	/* first check if the blocks can be rotated */
	for (int i = 0; i < 4; i++)
	{
		p = bpos + rotations[(currentRotation + 1)%4][i];
		
		if (p.x >= NUMCOLS || p.x < 0 ||
			p.y >= NUMROWS || p.y < 0 ||
			bgtiles -> isTile(p) ||
			(step > 0 && bgtiles -> isTile(p+Pos(0,1))))
		{
			return;
		}
	}


	if (++currentRotation >= 4)
		currentRotation = 0;
	for (int i = 0; i < 4; i++)
		tiles[i].pos = rotations[currentRotation][i];

	setPosition(BASEX, BASEY);
}

void Block::setPosition(int bx, int by)
{
	Pos p;
	for (int i = 0; i < 4; i++)
		if (tiles[i].sprite)
		{
			p = Pos(bx,by) + (bpos+tiles[i].pos)*TILESIZE + Pos(0,step*STEPSIZE);
			tiles[i].sprite -> setPosition(float(p.x),float(p.y));
		}
}