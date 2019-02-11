#ifndef PNG_DECODE
#define PNG_DECODE

#include <cstdint>
#include <vector>
#include <iostream>

#include "../dependencies/lodepng.h"

struct Image_rgb {
	unsigned int width;
  unsigned int height;
	std::vector<float> rgb;
};

struct Image_seperate_channels {
	unsigned int width;
  unsigned int height;
	std::vector<uint8_t> r;
	std::vector<uint8_t> g;
	std::vector<uint8_t> b;
};

Image_seperate_channels load_channels_png(const char* filename);
Image_rgb load_rgb_png(const char* filename);

#endif
