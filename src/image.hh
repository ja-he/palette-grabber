#pragma once

#include <filesystem>
#include <iostream>

#include <vector>

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
  Image(std::filesystem::path img_path);

  const Pixel& at(size_t line, size_t pixel) const;

  int get_width(void) const;
  int get_height(void) const;
  int get_n_channels(void) const;

private:
  int               width;
  int               height;
  int               n_channels;
  std::vector<Line> img;
};
