#include "Timer.h"
#include "Noise1D.h"
#include "Noise2d.h"
#include <algorithm>
#include <iostream>
#include <cmath>

// SEED GENERATOR
namespace Randomiser
{
	std::mt19937 mersenne{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };
}

void generateSeeds(std::vector<float>& terrain, std::uniform_real_distribution<float> engine)
{
	for (std::size_t i{ 0 }; i < terrain.size(); ++i)
		terrain[i] = engine(Randomiser::mersenne);
}

void shiftTerrain(std::vector<float>& terrain, std::uniform_real_distribution<float>& engine)
{
	for (std::size_t i{ terrain.size() - 1 }; i > 0; --i)
		terrain[i] = terrain[i - 1];
	terrain[0] = engine(Randomiser::mersenne);
}

int main()
{


	float deltaTime{ 0.0 };
	float timeTaken{ 0.0f };
	float scalingBias{ 2.0f };

	bool pauseNoise{ false };
	int nOctaves{ static_cast<int>(std::log(TERRAIN_SIZE) / std::log(2)) };

	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Line");

	// Generate Terrain
	std::uniform_real_distribution<float> random_engine(0, 1);
	std::vector<float> terrain(TERRAIN_SIZE * TERRAIN_SIZE);
	generateSeeds(terrain, random_engine);

	sf::Event event{};

	// Timer starts now
	Timer t;

	// Create noise array
	float *noise = new float[TERRAIN_SIZE * TERRAIN_SIZE]{};

	generateNoise2d(nOctaves, noise, terrain, scalingBias);

	sf::Texture texture{};
	texture.create(TERRAIN_SIZE * 2, TERRAIN_SIZE * 2);
	bool interpolate{ false };
	
	std::cout << "//////////\n\n2D TERRAIN GENERATOR\n\n//////////\n" <<
		"Press the arrow keys to adjust the image\n(UP): Increase Blend.\n(DOWN): Decrease Blend.\n"
		<< "(LEFT): Scale down.\n(RIGHT): Scale up.\nPRESS Q TO GENERATE A NEW IMAGE\n";
	while (window.isOpen())
	{
		deltaTime += t.elapsed();
		t.reset();
		timeTaken += deltaTime;

		while (TIME_PER_UPDATE <= timeTaken)
		{
			timeTaken -= TIME_PER_UPDATE;

			while (window.pollEvent(event))
			{
				switch (event.type)
				{
				case sf::Event::Closed:
					window.close();
					break;

				case sf::Event::KeyPressed:
					switch (event.key.code)
					{
					case sf::Keyboard::Up:
					{
						nOctaves += 1;

						generateNoise2d(nOctaves, noise, terrain, scalingBias);
						std::cout << "Octaves: " << nOctaves << '\n';

						break;
					}
					case::sf::Keyboard::Down:
					{
						nOctaves -= 1;

						if (nOctaves < 0)
							nOctaves = 0;

						generateNoise2d(nOctaves, noise, terrain, scalingBias);
						std::cout << "Octaves: " << nOctaves << '\n';

						break;
					}
					case sf::Keyboard::Q:
					{
						std::cout << "//////////\nNEW\n//////////\n";

						generateSeeds(terrain, random_engine);
						generateNoise2d(nOctaves, noise, terrain, scalingBias);

						break;
					}
					case sf::Keyboard::Left:
					{
						scalingBias -= 0.2f;
						generateNoise2d(nOctaves, noise, terrain, scalingBias);
						std::cout << "Scaling Factor: " << scalingBias << '\n';
						break;
					}
					case sf::Keyboard::Right:
					{
						scalingBias += 0.2f;

						generateNoise2d(nOctaves, noise, terrain, scalingBias);
						std::cout << "Scaling Factor: " << scalingBias << '\n';

						break;
					}
					case sf::Keyboard::B:
					{
						interpolate = !interpolate;
						drawNoise2d(noise, window, texture, interpolate);
						break;
					}
					case sf::Keyboard::Space: // For 1D
					{
						pauseNoise = !pauseNoise;  // Pause/Unpause
						break;
					}
					}
					break;
				}
			}
		}

		if (scalingBias < 0.2f)
			scalingBias = 0.2f;

		window.clear();

		drawNoise2d(noise, window, texture, interpolate);
		window.display();

	}
	delete[] noise;
}
