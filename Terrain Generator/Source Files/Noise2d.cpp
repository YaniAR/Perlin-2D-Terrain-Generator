#include "Noise2d.h"
#include "Functions.h"
#include <iostream>

void generateNoise2d(int& nOctaves, float noise[], std::vector<float>& terrain,
	float& scalingBias)
{
	if (nOctaves == 7)
		nOctaves = 0;

	for (int y = 0; y < PIXEL_HEIGHT; ++y)
	{
		for (int x = 0; x < PIXEL_WIDTH; ++x)
		{
			float Noise = 0.0f;
			float ScaleAcc = 0.0f;
			float Scale = 1.0f;

			for (int o = 0; o < nOctaves; o++)
			{
				// Shift by power of 2.
				int samplePeriod = PIXEL_WIDTH >> o;
				
				// Get sample points
				int xSample1 = (x / samplePeriod) * samplePeriod;
				int ySample1 = (y / samplePeriod) * samplePeriod;

				int xSample2 = (xSample1 + samplePeriod) % PIXEL_WIDTH; // wrap around x
				int ySample2 = (ySample1 + samplePeriod) % PIXEL_WIDTH; // warp around y

				float xBlend = (float)(x - xSample1) / (float)samplePeriod;
				float yBlend = (float)(y - ySample1) / (float)samplePeriod;

				float Sample1 = Interpolate(terrain[ySample1 * PIXEL_WIDTH + xSample1], terrain[ySample1 * PIXEL_WIDTH + xSample2], xBlend);
				float Sample2 = Interpolate(terrain[ySample2 * PIXEL_WIDTH + xSample1], terrain[ySample2 * PIXEL_WIDTH + xSample2], xBlend);

				ScaleAcc += Scale;
				Noise += (yBlend * (Sample2 - Sample1) + Sample1) * Scale;
				Scale /= scalingBias;
			}

			// Scale to seed range
			noise[y * PIXEL_WIDTH + x] = Noise / ScaleAcc;
		}
	}

}

void drawNoise2d(float noise[], sf::RenderWindow& window, sf::Texture& texture)
{
	sf::Image image{};
	sf::Sprite sprite{};

	image.create(PIXEL_WIDTH, PIXEL_HEIGHT, sf::Color(0, 0, 0));
	sf::Color color{};
	
	for (int y{ 0 }; y < PIXEL_HEIGHT; ++y)
	{
		for (int x{ 0 }; x < PIXEL_WIDTH; ++x)
		{
			int pixel_w{ (int)(noise[y * PIXEL_WIDTH + x] * 12.0f) };
			switch (pixel_w)
			{
			case 0: // DARK BLUE
				color = setColor(0, 119, 190);
				break;
			case 1:
				color = setColor(0, 0, 139);
				break;
			case 2:
				color = setColor(0, 0, 255);
				break;
			case 3:
				color = setColor(0, 0, 205);
				break;
			case 4:
				color = setColor(0, 105, 148);
				break;
			case 5: // GREEN
				color = setColor(34, 139, 34);
				break;
			case 6:
				color = setColor(0, 128, 0);
				break;
			case 7:
				color = setColor(0, 100, 0);
				break;
			case 8:
				color = setColor(124, 252, 0);
				break;
			case 9: // GREY
				color = setColor(194, 178, 128);
				break;
			case 10: // BROWN
				color = setColor(139, 69, 19);
				break;
			case 11:
				color = setColor(160, 82, 45);
				break;
			case 12:
				color = setColor(218, 165, 32);
				break;
			default:
				color = setColor(0, 0, 0);
				break;
			}
			//color = FloatToColour(noise[y * PIXEL_WIDTH + x]) -> for natural noise
			image.setPixel(x, y, color);
		}
	}

	texture.update(image);
	sprite.setTexture(texture);
	sprite.setScale(PIXEL_WIDTH, PIXEL_HEIGHT);
	window.draw(sprite);
}