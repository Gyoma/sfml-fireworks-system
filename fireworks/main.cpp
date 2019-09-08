#include <SFML/Graphics.hpp>
#include <list>

using namespace sf;
using namespace std;

class FireworksSystem : public Drawable, public Transformable
{
public:

	FireworksSystem(unsigned int count = 500) :
		m_position(0, 0),
		m_lifetime(seconds(3)),
		m_count(count)
	{}

	void launchFirework(RenderWindow& window)
	{
		m_position = window.mapPixelToCoords(Mouse::getPosition(window));

		Firework firework;

		firework.rocket.shape.setPosition(rand() % window.getSize().x, window.getSize().y + 50);
		firework.rocket.shape.setRadius(4);
		firework.rocket.shape.setFillColor(Color::Yellow);

		Vector2f rocketPos = firework.rocket.shape.getPosition();
		double angle = atan2(m_position.y - rocketPos.y, m_position.x - rocketPos.x);
		double speed = 400;

		firework.rocket.velocity = Vector2f(speed * cos(angle), speed * sin(angle));

		double distance = sqrt(pow(rocketPos.x - m_position.x, 2) + pow(rocketPos.y - m_position.y, 2));

		firework.rocket.lifetime = Time(seconds(distance / speed));

		firework.particles.clear();
		firework.particles.resize(m_count);
		firework.particlesLeft = m_count;

		firework.vertices.setPrimitiveType(Points);
		firework.vertices.clear();
		firework.vertices.resize(m_count);

		for (int i = firework.particles.size() - 1; i >= 0; i--)
		{
			float angle = (rand() % 360) * 3.14f / 180.f;
			float speed = (rand() % 120);
			firework.particles[i].velocity = Vector2f(cos(angle) * speed, sin(angle) * speed);
			firework.particles[i].lifetime = milliseconds((rand() % 1000) + 1000);

			firework.vertices[i].position = m_position;

			switch (rand() % 6)
			{
			case 0:
				firework.vertices[i].color = Color::Blue;
				break;
			case 1:
				firework.vertices[i].color = Color::Cyan;
				break;
			case 2:
				firework.vertices[i].color = Color::Green;
				break;
			case 3:
				firework.vertices[i].color = Color::Yellow;
				break;
			case 4:
				firework.vertices[i].color = Color::Red;
				break;
			case 5:
				firework.vertices[i].color = Color::White;
			}
		}

		fireworks.push_front(firework);
	}

	void update(Time elapsed)
	{
		for (auto firework = fireworks.begin(); firework != fireworks.end(); )
		{
			if (!firework->rocket.gotToPosition)
			{
				Rocket& r = firework->rocket;

				r.lifetime -= elapsed;

				if (r.lifetime <= Time::Zero)
				{
					r.gotToPosition = true;
					continue;
				}

				r.shape.setPosition(r.shape.getPosition() + r.velocity * elapsed.asSeconds());
			}
			else
				for (int j = firework->particles.size() - 1; j >= 0; j--)
				{
					if (!firework->particles[j].dead)
					{
						Particle& p = firework->particles[j];
						p.lifetime -= elapsed;

						if (p.lifetime <= Time::Zero)
						{
							firework->particles[j].dead = true;
							firework->particlesLeft--;
							continue;
						}

						firework->particles[j].velocity.y += 0.1;

						firework->vertices[j].position += p.velocity * elapsed.asSeconds();

						if (firework->particles[j].lifetime < seconds(0.2))
							firework->vertices[j].color.a = static_cast<Uint8>(p.lifetime.asSeconds() / m_lifetime.asSeconds() * 255);
					}
				}

			if (firework->particlesLeft == 0)
				firework = fireworks.erase(firework);
			else
				firework++;
		}
	}

private:

	virtual void draw(RenderTarget& target, RenderStates states) const
	{
		states.transform *= getTransform();

		states.texture = NULL;

		for (const Firework& firework : fireworks)
		{
			if (!firework.rocket.gotToPosition)
				target.draw(firework.rocket.shape);
			else
				target.draw(firework.vertices, states);
		}
	}

private:

	struct Particle
	{
		Vector2f velocity;
		Time lifetime;
		bool dead = false;
	};

	struct Rocket
	{
		CircleShape shape;
		Vector2f velocity;
		Time lifetime;
		bool gotToPosition = false;
	};

	struct Firework
	{
		Rocket rocket;
		vector<Particle> particles;
		unsigned int particlesLeft;
		VertexArray vertices;
	};


	Vector2f m_position;
	Time m_lifetime;
	int m_count;
	list<Firework> fireworks;
};

int main()
{
	RenderWindow window(VideoMode(1200, 700), "Particles");

	FireworksSystem particles;

	Clock clock;

	while (window.isOpen())
	{

		Event event;

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::MouseButtonPressed)
				particles.launchFirework(window);
		}


		Time elapsed = clock.restart();
		particles.update(elapsed);

		window.clear();
		window.draw(particles);
		window.display();
	}

	return 0;
}
