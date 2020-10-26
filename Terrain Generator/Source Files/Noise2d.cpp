#include "Noise2d.h"
#include "Functions.h"
#include <iostream>

void generateNoise2d(int& nOctaves, float noise[], std::vector<float>& terrain,
	float& scalingBias)
{
	if (nOctaves == 7)
		nOctaves = 0;

	for (int y = 0; y < TERRAIN_SIZE; ++y)
	{
		for (int x = 0; x < TERRAIN_SIZE; ++x)
		{
			float Noise = 0.0f;
			float ScaleAcc = 0.0f;
			float Scale = 1.0f;

			for (int o = 0; o < nOctaves; o++)
			{
				// Shift by power of 2.
				int samplePeriod = TERRAIN_SIZE >> o;
				
				// Get sample points
				int xSample1 = (x / samplePeriod) * samplePeriod;
				int ySample1 = (y / samplePeriod) * samplePeriod;

				int xSample2 = (xSample1 + samplePeriod) % TERRAIN_SIZE; // wrap around x
				int ySample2 = (ySample1 + samplePeriod) % TERRAIN_SIZE; // warp around y

				float xBlend = (float)(x - xSample1) / (float)samplePeriod;
				float yBlend = (float)(y - ySample1) / (float)samplePeriod;

				float Sample1 = Interpolate(terrain[ySample1 * TERRAIN_SIZE + xSample1], terrain[ySample1 * TERRAIN_SIZE + xSample2], xBlend);
				float Sample2 = Interpolate(terrain[ySample2 * TERRAIN_SIZE + xSample1], terrain[ySample2 * TERRAIN_SIZE + xSample2], xBlend);

				ScaleAcc += Scale;
				Noise += (yBlend * (Sample2 - Sample1) + Sample1) * Scale;
				Scale /= scalingBias;
			}

			// Scale to seed range
			noise[y * TERRAIN_SIZE + x] = Noise / ScaleAcc;
		}
	}

}
/**/

void bilinearInterpolate(float noise[], sf::RenderWindow& window, sf::Texture& texture)
{
	sf::Sprite sprite{};
	texture.update(window);
	sf::Image image{ texture.copyToImage() };

	sf::Color tL{}, tR{}, bL{}, bR{};
	sf::Vector2i tLPos{}, tRPos{}, bLPos{}, bRPos{};

	for (int y{ 0 }; y < TERRAIN_SIZE * 2; ++y)
	{
		for (int x{ 0 }; x < TERRAIN_SIZE * 2; ++x)
		{
			getSquarePoints(bLPos, bRPos, tLPos, tRPos, bL, bR, tL, tR, x, y, image);
			
			int totalDistanceX{ tRPos.x - tLPos.x };
			int totalDistanceY{ tRPos.y - bRPos.y };
			int differenceToLeft{};
			int differenceToRight{};
			
			sf::Color currentPixel{0, 0, 0};
			for (int pixelY{ tLPos.y }; pixelY <= bLPos.y; ++pixelY)
			{
				for (int pixelX{ tLPos.x }; pixelX <= tRPos.x; ++pixelX)
				{
					differenceToLeft = pixelX - tLPos.x;
					differenceToRight = tRPos.x - pixelX;
					
					int differenceToTop{ tLPos.y - pixelY };
					int differenceToBottom{ pixelY - bLPos.y };

					int rTop = (tL.r * differenceToRight + tR.r * differenceToLeft) / totalDistanceX;
					int gTop = (tL.g * differenceToRight + tR.g * differenceToLeft) / totalDistanceX;
					int bTop = (tL.b * differenceToRight + tR.b * differenceToLeft) / totalDistanceX;

					int rBot = (bL.r * differenceToRight + bR.r * differenceToLeft) / totalDistanceX;
					int gBot = (bL.g * differenceToRight + bR.g * differenceToLeft) / totalDistanceX;
					int bBot = (bL.b * differenceToRight + bR.b * differenceToLeft) / totalDistanceX;

					currentPixel.r = static_cast<sf::Uint8>((rBot * differenceToTop + rTop * differenceToBottom) / totalDistanceY);
					currentPixel.g = static_cast<sf::Uint8>((gBot * differenceToTop + gTop * differenceToBottom) / totalDistanceY);
					currentPixel.b = static_cast<sf::Uint8>((bBot * differenceToTop + bTop * differenceToBottom) / totalDistanceY);

					image.setPixel(pixelX, pixelY, currentPixel);
				}
			}
		}
	}
	std::cout << "done\n";

	texture.update(image);
	sprite.setTexture(texture);
	window.draw(sprite);
}

void drawDots(sf::RenderWindow& window)
{
	sf::RectangleShape dot(sf::Vector2f(LINETHICKNESS, LINETHICKNESS));
	dot.setFillColor(sf::Color::Black);

	for (int y{ 0 }; y < TERRAIN_SIZE * 2; ++y)
	{
		for (int x{ 0 }; x < TERRAIN_SIZE * 2; ++x)
		{
			dot.setPosition(static_cast<float>(((TERRAIN_SIZE * x + TERRAIN_SIZE) / 2) - TERRAIN_SIZE / 4), 
				static_cast<float>(((TERRAIN_SIZE * y + TERRAIN_SIZE) / 2) - TERRAIN_SIZE / 4));
			window.draw(dot);
		}
	}
}

void drawNoise2d(float noise[], sf::RenderWindow& window, sf::Texture& texture, bool interpolate)
{
	sf::Color color{};

	sf::RectangleShape cell{ sf::Vector2f(TERRAIN_SIZE / 2, TERRAIN_SIZE / 2) };
	float pixel_w{};
	for (int y{ 0 }; y < TERRAIN_SIZE * 2; ++y)
	{
		for (int x{ 0 }; x < TERRAIN_SIZE * 2; ++x)
		{
			pixel_w = nearestNeighbourInterpolate(noise, x, y);
			pixel_w = std::pow(pixel_w, 2);
			if (WATER_LEVEL > pixel_w)
			{
				color = setColor(0, std::round(127 * (pixel_w / WATER_LEVEL)), 192 + std::round(63 * (pixel_w - WATER_LEVEL)));
			}
			else if (SAND_LEVEL > pixel_w)
			{
				color = setColor(255, 255, 63);
			}
			else if (GRASS_LEVEL > pixel_w)
			{
				color = setColor(0, 255 - std::round(128 * ((pixel_w - SAND_LEVEL) / GRASS_LEVEL)), 64 - std::round(31 * ((pixel_w - SAND_LEVEL) / GRASS_LEVEL)));
			}
			else if (SNOW_LEVEL > pixel_w)
			{
				color = setColor(95, 64 + std::round(31 * ((pixel_w - GRASS_LEVEL) / SNOW_LEVEL)), 64 + std::round(31 * ((pixel_w - GRASS_LEVEL) / SNOW_LEVEL)));
			}
			else
			{
				color = setColor(192 + std::round(63 * ((pixel_w - SNOW_LEVEL) / SNOW_LEVEL)), 
					192 + std::round(63 * (pixel_w - SNOW_LEVEL)), 
					192 + std::round(63 * (pixel_w - SNOW_LEVEL)));
			}

			cell.setFillColor(color);
			cell.setPosition(x * (PIXEL_WIDTH / 2), y * (PIXEL_HEIGHT / 2));
			window.draw(cell);
		}
	}
}
