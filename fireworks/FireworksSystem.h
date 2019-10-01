#pragma once
#include <SFML/Graphics.hpp>
#include <list>
#include <vector>

class FireworksSystem : public sf::Drawable, public sf::Transformable
{
public:

	FireworksSystem(unsigned int count = 500);

	void launchFirework(sf::RenderWindow& window);

	void update(sf::Time elapsed);

private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:

	struct Particle
	{
		sf::Vector2f velocity;
		sf::Time lifetime;
		bool dead = false;
	};

	struct Rocket
	{
		sf::CircleShape shape;
		sf::Vector2f velocity;
		sf::Time lifetime;
		bool gotToPosition = false;
	};

	struct Firework
	{
		Rocket rocket;
		std::vector<Particle> particles;
		unsigned int particlesLeft;
		sf::VertexArray vertices;
	};


	sf::Vector2f m_position;
	sf::Time m_lifetime;
	int m_count;
	std::list<Firework> fireworks;
};