#include "Noise1D.h"
#include <vector>

void drawLine(short begin_x, short begin_y, short end_x, short end_y,
	sf::RenderWindow& window, const sf::Color color)
{

	unsigned short max_line_length{ static_cast<unsigned short>(std::max(1, std::max(std::abs(end_x - begin_x),
		std::abs(end_y - begin_y)))) };

	// Total dots needed to draw
	float x_step{ static_cast<float>(end_x - begin_x) / max_line_length };
	float y_step{ static_cast<float>(end_y - begin_y) / max_line_length };

	sf::RectangleShape point(sf::Vector2f(LINETHICKNESS, LINETHICKNESS));
	point.setFillColor(color);

	for (unsigned short i = 0; i <= max_line_length; ++i)
	{
		point.setPosition((begin_x + x_step * i) - std::floor(0.5 * LINETHICKNESS),
			((begin_y + y_step * i) - std::floor(0.5 * LINETHICKNESS)));

		window.draw(point);
	}
}

// create Noise
void generateNoise(const int nOctaves, float(&noise)[TOTAL_ELEMENTS], std::vector<float>& terrain,
	float& scalingBias)
{
	for (int i{ 0 }; i < TOTAL_ELEMENTS; i++)
	{
		float Noise = 0.0f;
		float ScaleAcc = 0.0f;
		float Scale = 1.0f;

		for (int o{ 0 }; o <= nOctaves; o++)
		{
			// Shift by powers of 2.
			int Pitch = TERRAIN_SIZE >> o;

			//Picks two different points in the x axis
			int Sample1 = (i / Pitch) * Pitch;
			int Sample2 = (Sample1 + Pitch) % TERRAIN_SIZE;

			float Blend = (float)(i - Sample1) / Pitch;

			float mainSample = (1.0f - Blend) * terrain[Sample1] + Blend * terrain[Sample2];

			ScaleAcc += Scale;
			Noise += mainSample * Scale;
			Scale /= scalingBias;
		}
		noise[i] = (Noise / ScaleAcc);
	}
}

// Continuous shifting animation for 1d noise.
void smoothNoise(float(&noise)[])
{
	float temp = noise[TOTAL_ELEMENTS - 1];
	for (int i{ TOTAL_ELEMENTS - 1 }; i > 0; --i)
		noise[i] = noise[i - 1];
	noise[0] = temp;

}
/**/