#ifndef GAME
#define GAME

#include <SFML/Graphics.hpp>

#define NUMSTATES 4

class Block;
class BGTiles;
class GameState;

struct Pos;

void setText(sf::Text & text, sf::Font & font, Pos pos, int CharSize, const char * str, sf::Color color);

class Game
{
public:
	BGTiles * bgtiles;
	Block * currentBlock, * nextBlock;
	sf::Texture * tileTexture;
	sf::Texture * backgroundTexture;
	sf::Sprite * background;
	sf::RenderWindow * window;
	GameState * stack[NUMSTATES];
	int currentState;
	bool reset, stateEnded, paused;
	unsigned int score, highscore;
	sf::Text scoreText, hsText, controls, start, title, loser;
	sf::Font font;

	Game(sf::RenderWindow & w);
	void exit();

	void handleEvent(sf::Event e);
	void run();
	void draw();

	void pushState(GameState * state);
	void popState();
};

#endif