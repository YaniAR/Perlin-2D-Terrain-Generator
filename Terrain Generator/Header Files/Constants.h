#ifndef CONSTANTS_H
#define CONSTANTS_H

constexpr float TIME_PER_UPDATE{ 1.0f };
constexpr int LINETHICKNESS{ 5 };
constexpr int TERRAIN_SIZE{ 32 };
constexpr int SCREEN_WIDTH{ 1024 };	
constexpr int SCREEN_HEIGHT{ 1024 };
constexpr int PIXEL_WIDTH{ SCREEN_WIDTH / TERRAIN_SIZE };
constexpr int PIXEL_HEIGHT{ SCREEN_HEIGHT / TERRAIN_SIZE };
constexpr int TOTAL_ELEMENTS{ PIXEL_WIDTH * PIXEL_HEIGHT };

#endif
