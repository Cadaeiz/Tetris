#ifndef GAMESTATE
#define GAMESTATE

#include <SFML/Graphics.hpp>
class Game;

#define FLASHLEN 30
#define NUMFLASHES 5	

class GameState
{
public:
	Game * game;
	GameState(Game * g);

	virtual void handleEvent(sf::Event e) = 0;
	virtual void run() = 0;
	virtual void draw(sf::RenderWindow * window) = 0;

	void pushState(GameState * state);
	void popState();
};

class StartState : public GameState 
{
public:
	StartState(Game * g) : GameState(g) { }
	void handleEvent(sf::Event e);
	/* nothing to run in start state */
	void run() { }
	void draw(sf::RenderWindow * window);
};

class ActiveBlockState : public GameState 
{
public:
	ActiveBlockState(Game * g) : GameState(g) { }
	void handleEvent(sf::Event e);
	void run();
	void draw(sf::RenderWindow * window);
};

class RemovingRowsState : public GameState 
{
	int flashCount1, flashCount2;
public:
	RemovingRowsState(Game * g);
	void handleEvent(sf::Event e);
	void run();
	void draw(sf::RenderWindow * window) { }
};

class GameOverState : public GameState 
{
public:
	GameOverState(Game * g);
	void handleEvent(sf::Event e);
	/* nothing to run in gameover */
	void run() { }
	void draw(sf::RenderWindow * window);
};

#endif