#include <filesystem>
#include <iostream>

#include <vector>

#include <algorithm>
#include <numeric>

#include <cassert>

#include <stdint.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

struct Pixel
{
  uint8_t r = 0;
  uint8_t g = 0;
  uint8_t b = 0;
};
typedef std::vector<Pixel> Line;

class Image
{
public:
  Image(std::filesystem::path img_path)
  {
    uint8_t* rgb_image = stbi_load(
      img_path.c_str(), &this->width, &this->height, &this->n_channels, 3);
    if (n_channels != 3) {
      std::cerr << "WARNING (reading in image): number of channels in image is "
                   "not 3 but "
                << n_channels << std::endl;
    }

    this->img.resize(height);
    std::for_each(
      img.begin(), img.end(), [this](auto& line) { line.resize(width); });

    for (int li = 0; li < height; li++) {
      for (int pi = 0; pi < width; pi++) {
        this->img[li][pi] = { rgb_image[(li * width * 3) + (pi * 3) + (0)],
                              rgb_image[(li * width * 3) + (pi * 3) + (1)],
                              rgb_image[(li * width * 3) + (pi * 3) + (2)] };
      }
    }

    stbi_image_free(rgb_image);
  };

  const Pixel& at(size_t line, size_t pixel) const
  {
    return this->img[line][pixel];
  };

  int get_width(void) const { return this->width; };
  int get_height(void) const { return this->height; };
  int get_n_channels(void) const { return this->n_channels; };

private:
  int               width;
  int               height;
  int               n_channels;
  std::vector<Line> img;
};

int
main(int argc, char* argv[])
{
  assert(argc > 1);

  Image img(argv[1]);
  printf("read in a %d by %d image with %d channels.\n",
         img.get_width(),
         img.get_height(),
         img.get_n_channels());

  return 0;
}
