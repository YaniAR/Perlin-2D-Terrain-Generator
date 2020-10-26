#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <SFML/Graphics/Color.hpp>

template <typename T>
inline T Interpolate(T& x0, T& x1, T& alpha)
{
	return (1.0 - alpha) * x0 + (alpha * x1);
};

sf::Color setColor(int r, int g, int b)
{
	return sf::Color(sf::Uint8(r), sf::Uint8(g), sf::Uint8(b));
}
sf::Color FloatToColour(float& value)
{
	return sf::Color(sf::Uint8(value * 255.f), sf::Uint8(value * 255.f), sf::Uint8(value * 255.f));
}

template <typename T>
T average(const T& a, const T& b)
{
	return ((a + b) / 2);
}

int RGBToDec(sf::Color c)
{
	return (c.r << 16) | (c.g << 8) | c.b;
}

sf::Color RGBFromDec(int d)
{
	sf::Color temp{};
	temp.r = (d >> 16) & 255;
	temp.g = (d >> 8) & 255;
	temp.b = d & 255;
	return temp;
}

float nearestNeighbourInterpolate(float noise[], int x, int y)
{
	if (x % 2 == 0 && y % 2 == 0)
		return (noise[(y * TERRAIN_SIZE + x) / 2]);
	else if (x % 2 == 1 && y % 2 == 0)
		return average(noise[(y * TERRAIN_SIZE + x + 1) / 2], noise[(y * TERRAIN_SIZE + x - 1) / 2]);
	else if (x % 2 == 0 && y % 2 == 1)
		return average(noise[(y * TERRAIN_SIZE + x + TERRAIN_SIZE) / 2], noise[(y * TERRAIN_SIZE + x - TERRAIN_SIZE) / 2]);
	else if (x % 2 == 1 && y % 2 == 1)
		return average(noise[(y * TERRAIN_SIZE + x + TERRAIN_SIZE + 1) / 2], noise[(y * TERRAIN_SIZE + x - TERRAIN_SIZE - 1) / 2]);
}

void getSquarePoints(sf::Vector2i& bLPos, sf::Vector2i& bRPos, sf::Vector2i& tLPos, sf::Vector2i& tRPos, sf::Color& bL, sf::Color& bR, sf::Color& tL, sf::Color& tR, int x, int y, const sf::Image& image)
{
	bLPos.x, tLPos.x = (((TERRAIN_SIZE * x + TERRAIN_SIZE) / 2) - TERRAIN_SIZE / 4);
	bRPos.x, tRPos.x = tLPos.x + (TERRAIN_SIZE / 2);
	tLPos.y, tRPos.y = (((PIXEL_HEIGHT * y + PIXEL_HEIGHT) / 2) - PIXEL_HEIGHT / 4);
	bLPos.y, bRPos.y = tLPos.y + (TERRAIN_SIZE / 2);

	tL = image.getPixel(tLPos.x, tLPos.y);
	tR = image.getPixel(tRPos.x, tRPos.y);
	bL = image.getPixel(bLPos.x, bLPos.y);
	bR = image.getPixel(bRPos.x, bRPos.y);
}

#endif
