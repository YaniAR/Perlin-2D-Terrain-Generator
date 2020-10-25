#ifndef NOISE1D_H
#define NOISE1D_H

#include "Constants.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>

void drawLine(short begin_x, short begin_y, short end_x, short end_y,
	sf::RenderWindow& window, const sf::Color color);
void generateNoise(const int nOctaves, float(&noise)[TERRAIN_SIZE], std::vector<float>& terrain,
	float& scalingBias);
void smoothNoise(float (&noise)[TERRAIN_SIZE]);

#endif