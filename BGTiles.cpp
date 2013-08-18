#include "BGTiles.hpp"
#include "Block.hpp"

/* Constructor initializes everything to 0 */
BGTiles::BGTiles()
{
	for (int i = 0; i < NUMROWS; i++)
	{
		rowSum[i] = 0;
		for (int j = 0; j < NUMCOLS; j++)
			sprites[i][j] = 0;
	}
	step = 0;

}

/* destructor takes care of dynamically allocated tiles */
BGTiles::~BGTiles()
{
	/* reset removes all sprites from heap */
	reset();
}

/* delete any existing sprites in the background */
void BGTiles::reset()
{
	for (int i = 0; i < NUMROWS; i++)
	{
		for (int j = 0; j < NUMCOLS; j++)
			if (sprites[i][j])
			{
				delete sprites[i][j];
				sprites[i][j] = 0;
			}

		rowSum[i] = 0;
	}
	step = 0;
}

/* update a tile in the background with the given one
 * if there is already a tile in the space, return false
 */
bool BGTiles::addTile(Tile t)
{
	if (!sprites[t.y][t.x])
	{
		sprites[t.y][t.x] = t.sprite;
		rowSum[t.y]++;
		return true;
	}
	else
		return false;
}

/* check if any row has been filled */
bool BGTiles::checkRows()
{
	for (int i = 0; i < NUMROWS; i++)
		if (rowSum[i] == NUMCOLS)
			return true;

	return false;
}

/* remove all filled rows */
int BGTiles::removeRows()
{
	int score = 1;
	for (int i = 0; i < NUMROWS; i++)
		if (rowSum[i] == NUMCOLS)
		{
			score *= 2;
			/* reset the rowSum and delete all sprites in the row */
			rowSum[i] = 0;
			for (int j = 0; j < NUMCOLS; j++)
			{
				delete sprites[i][j];
				sprites[i][j] = 0;
			}
		}

	return (score / 2) * 100;
}

/* shift all tiles above an erased row down until all empty rows are filled again
 * if there are no tiles to move, return false 
 */
bool BGTiles::descend()
{
	int i;
	/* find the first row with no tiles */
	for (i = NUMROWS - 1; i >= 0 && rowSum[i]; i--);
	
	/* find the first nonempty row above an empty one */
	while (--i >= 0 && rowSum[i] == 0);

	if (i < 0) return false;

	/* advance all rows by one step */
	if(++step == STEPS_PER_BLOCK)
		step = 0;

	while (i >= 0)
	{
		/* for every unempty row after */
		if (rowSum[i])
		{
			for (int j = 0; j < NUMCOLS; j++)
				if (sprites[i][j])
				{
					/* move the tiles down one step */
					sprites[i][j] -> move(sf::Vector2f(0,STEPSIZE));

					/* if a full square has passed, move tiles to next square */
					if (!step)
					{
						sprites[i+1][j] = sprites[i][j];
						sprites[i][j] = 0;
					}
				}

			if (!step)
			{
				rowSum[i+1] = rowSum[i];
				rowSum[i] = 0;
			}
		}
					
		i--;
	}

	return true;
}

/* make all filled rows visible */
void BGTiles::blink()
{
	sf::Color color;
	/* cycle through all sprites in filled rows */
	for (int i = 0; i < NUMROWS; i++)
		if (rowSum[i] == NUMCOLS)
			for (int j = 0; j < NUMCOLS; j++)
			{
				/* set the opacity of the sprite to max */
				color = sprites[i][j] -> getColor();
				color.a = 255 - color.a;
				sprites[i][j] -> setColor(color);
			}
}

bool BGTiles::isTile(int x, int y)
{
	return sprites[y][x];
}

void BGTiles::draw(sf::RenderWindow & window)
{
	for (int i = 0; i < NUMROWS; i++)
		for (int j = 0; j < NUMCOLS; j++)
			if (sprites[i][j])
				window.draw(*sprites[i][j]);
}