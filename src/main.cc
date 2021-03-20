#include <iostream>

#include <algorithm>
#include <numeric>

#include <cassert>

#include <stdint.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int
main(int argc, char* argv[])
{
  assert(argc > 1);

  int width, height, n_channels;
  uint8_t* rgb_image = stbi_load(argv[1], &width, &height, &n_channels, 3);
  assert(n_channels == 3);

  stbi_image_free(rgb_image);

  return 0;
}
