#include <SFML/Graphics.hpp>
#include "Game.hpp"
#include <sstream>

#define SCREENW 800
#define SCREENH 600

#define FPS 60
#define PERIOD (1.0/FPS)


int main()
{
	sf::RenderWindow window(sf::VideoMode(SCREENW, SCREENH), "Tetris");
	sf::Clock clock;
	sf::Time elapsed;

	sf::Font font;
	font.loadFromFile("arial.ttf");
	sf::Text fps;
	fps.setFont(font);
	fps.setCharacterSize(15);
	fps.setColor(sf::Color::Green);
	std::stringstream ss;

	Game game = Game(window);

	while (window.isOpen())
	{
		elapsed = clock.getElapsedTime();
		/* wait 1/FPS seconds to rewrite to screen */
		if (elapsed.asSeconds() >= PERIOD)
		{
			ss.str("");
			ss << "FPS: " << (1 / elapsed.asSeconds());
			fps.setString(ss.str());

			clock.restart();
			window.clear();
			game.run();
			game.draw();
			//window.draw(fps);
			window.display();
		}

		sf::Event e;
		while (window.pollEvent(e))
		{
			switch(e.type)
			{
			case sf::Event::Closed:
				game.cleanup();
				window.close();
				break;
			default:
				game.handleEvent(e);
			}
		}
	}

	return 0;
}