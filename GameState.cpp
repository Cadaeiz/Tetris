#include "Game.hpp"
#include "GameState.hpp"
#include "Block.hpp"
#include "BGTiles.hpp"

GameState::GameState(Game * g)
{
	game = g;
}
void GameState::pushState(GameState * state)
{
	game -> pushState(state);
}

void GameState::popState()
{
	game -> stateEnded = true;
}




void StartState::handleEvent(sf::Event e)
{
	/* pressing enter starts a new game */
	if (e.type == sf::Event::KeyPressed &&
		e.key.code == sf::Keyboard::Return)
	{
		pushState(new ActiveBlockState(game));
	}
}

void StartState::draw(sf::RenderWindow * window)
{
	game -> start.setString("Press Enter to Start");
	game -> start.setCharacterSize(30);

	window -> draw(game -> start);
	window -> draw(game -> controls);
}



void ActiveBlockState::handleEvent(sf::Event e)
{
	if (e.type == sf::Event::KeyPressed)
		switch(e.key.code)
		{
		case sf::Keyboard::Left:
			if (game -> currentBlock)
				game -> currentBlock -> moveLeft();
			break;
		case sf::Keyboard::Right:
			if (game -> currentBlock)
				game -> currentBlock -> moveRight();
			break;

		case sf::Keyboard::Up:
			if (game -> currentBlock)
				game -> currentBlock -> rotate();
			break;
		case sf::Keyboard::Down:
			/* move block down if possible */
			if (game -> currentBlock &&
				!game -> currentBlock -> descendBlock())
			{
				/* if block has stopped, delete the block item */
				delete game -> currentBlock;
				game -> currentBlock = 0;
				/* if there are any filled rows */
				if (game -> bgtiles -> checkRows())
					pushState(new RemovingRowsState(game));
			}

			break;
		case sf::Keyboard::Return:
			/* pause the game */
			game -> paused = true;
			break;
		}
}

void ActiveBlockState::run()
{
	/* add block if there is no active one */
	if (!game -> currentBlock)
	{
		game -> currentBlock = game -> nextBlock;
		game -> nextBlock = new Block(game -> bgtiles, game -> tileTexture);

		/* if block does not fit, call gameover */
		if (!game -> currentBlock -> makeActive())
		{
			delete game -> currentBlock;
			game -> currentBlock = 0;
			pushState(new GameOverState(game));
			return;
		}
	}

	/* check delay before block falls */

	/* move current block if possible */
	if (!game -> currentBlock -> descend())
	{
		/* if block has stopped, delete the block item */
		delete game -> currentBlock;
		game -> currentBlock = 0;
		/* if there are any filled rows */
		if (game -> bgtiles -> checkRows())
			pushState(new RemovingRowsState(game));
	}
}


void ActiveBlockState::draw(sf::RenderWindow * window)
{
	if (game -> currentBlock)
		game -> currentBlock -> draw(*window);
}

RemovingRowsState::RemovingRowsState(Game * g) : GameState(g)
{
	flashCount1 = 0;
	flashCount2 = 0;
}

void RemovingRowsState::handleEvent(sf::Event e)
{
	if (e.type == sf::Event::KeyPressed &&
		e.key.code == sf::Keyboard::Return)
	{
		/* pause the game */
		game -> paused = true;
	}
	
}

void RemovingRowsState::run()
{
	if (flashCount2 < NUMFLASHES)
	{
		if (flashCount1-- == 0)
		{
			flashCount1 = FLASHLEN - 1;
			game -> bgtiles -> blink();
			/* add points to score for rows removed */
			if (++flashCount2 == NUMFLASHES)
				game -> score += game -> bgtiles -> removeRows();
		}
	}
	/* after blinking, move other tiles down */
	else if (!game -> bgtiles -> descend())
		popState();
}

GameOverState::GameOverState(Game * g) : GameState(g)
{
	if ((game -> score) > (game -> highscore))
		game -> highscore = game -> score;
}

void GameOverState::handleEvent(sf::Event e)
{
	/* if enter is pressed, return to start state */
	if (e.type == sf::Event::KeyPressed &&
		e.key.code == sf::Keyboard::Return)
	{
		game -> reset = true;
	}

}

void GameOverState::draw(sf::RenderWindow * window)
{
	game -> start.setString("Press Enter to Continue");
	game -> start.setCharacterSize(25);

	window -> draw(game -> start);

	window -> draw(game -> loser);

}