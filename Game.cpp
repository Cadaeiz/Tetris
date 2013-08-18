#include "Game.hpp"
#include "GameState.hpp"
#include "Block.hpp"
#include "BGTiles.hpp"
#include <ctime>
#include <fstream>
#include <sstream>
using namespace std;


void setText(sf::Text & text, sf::Font & font, Pos pos, int CharSize = 0, const char * str = "", sf::Color color = sf::Color::White)
{
	text.setFont(font);
	text.setPosition(pos.x,pos.y);
	if (CharSize)
		text.setCharacterSize(CharSize);
	text.setString(str);
	text.setColor(color);
}

/* initialize all objects in game */
Game::Game(sf::RenderWindow & w)
{
	window = &w;

	tileTexture = new sf::Texture();
	tileTexture -> loadFromFile("tile.png");

	backgroundTexture = new sf::Texture();
	backgroundTexture -> loadFromFile("background.png");
	background = new sf::Sprite();
	background -> setTexture(*backgroundTexture);

	bgtiles = new BGTiles();
	currentBlock = 0;
	reset = stateEnded = paused = false;
	srand(time(NULL));

	nextBlock = new Block(bgtiles, tileTexture);
	
	
	score = highscore = 0;

	ifstream hs("highscore.txt");
	if (hs)
	{
		hs >> highscore;
		hs.close();
	}
	
	font.loadFromFile("arial.ttf");
	setText(scoreText,font,Pos(442,380),25);
	setText(hsText,font,Pos(442,488),25);
	setText(controls,font,Pos(36,444),20,"Controls:\n\nMove block:\t Left,Right,Down\nRotate:\t\t\tUp\nPause:\t\t\t Enter");
	setText(start,font,Pos(50,300));
	setText(title,font,Pos(450,50),50,"Quadruplos",sf::Color::Blue);
	setText(loser,font,Pos(50,100),50,"Game Over",sf::Color::Red);

	/* start with only the startstate on the stack */
	currentState = 0;
	stack[0] = new StartState(this);
}

void Game::handleEvent(sf::Event e)
{ 
	if (paused)
	{
		if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Return)
			paused = false;

		return;
	}

	stack[currentState] -> handleEvent(e); 
}


void Game::pushState(GameState * gs)
{
	stack[++currentState] = gs;
}

void Game::popState()
{
	delete stack[currentState];
	stack[currentState--] = 0;
	if (currentState == -1)
		exit();
}

void Game::run()
{
	if (reset)
	{
		/* reset all objects and state to 0 */
		reset = false;
		bgtiles -> reset();
		if (score == highscore)
		{
			ofstream hs("highscore.txt");
			hs << score;
			hs.close();
		}
		score = 0;
		while(currentState)
			popState();
	}
	else if (stateEnded)
	{
		stateEnded = false;
		popState();
	}
	if (!paused)
		stack[currentState] -> run();
}

void Game::draw()
{
	/* draw background image and current tiles */
	window -> draw(*background);
	nextBlock -> draw(*window);
	bgtiles -> draw(*window);

	/* draw 'Pause' if currently paused */

	/* draw current and high score */
	stringstream ss;

	ss.str("");
	ss << "High Score:		";
	ss.fill('0');
	ss.width(8);
	ss << highscore;
	hsText.setString(ss.str());
	window -> draw(hsText);

	ss.str("");
	ss << "Current Score:	";
	ss.fill('0');
	ss.width(8);
	ss << score;
	scoreText.setString(ss.str());
	window -> draw(scoreText);

	window -> draw(title);

	/* call draw function of current state */
	stack[currentState] -> draw(window); 
}
void Game::exit()
{
}