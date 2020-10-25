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


#endif