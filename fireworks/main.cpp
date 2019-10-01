#include <SFML/Graphics.hpp>
#include "FireworksSystem.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(1200, 700), "Fireworks");

	FireworksSystem FS;

	sf::Clock clock;

	while (window.isOpen())
	{

		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::MouseButtonPressed)
				FS.launchFirework(window);
		}


		sf::Time elapsed = clock.restart();
		FS.update(elapsed);

		window.clear();
		window.draw(FS);
		window.display();
	}

	return 0;
}
