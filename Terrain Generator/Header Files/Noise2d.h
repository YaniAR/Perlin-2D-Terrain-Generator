#ifndef NOISE2D_H
#define NOISE2D_H

#include "Constants.h"
#include <vector>
#include <random>
#include <SFML/Graphics.hpp>

void generateNoise2d(int& nOctaves, float noise[], std::vector<float>& terrain,
	float& scalingBias);

void drawNoise2d(float noise[], sf::RenderWindow& window, sf::Texture& texture);

#endif