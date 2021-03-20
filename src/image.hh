#pragma once

#include <filesystem>
#include <iostream>

#include <string>
#include <vector>

struct Pixel
{
  uint8_t     r = 0;
  uint8_t     g = 0;
  uint8_t     b = 0;
  std::string to_hex(void) const;
  uint32_t to_uint(void) const;
};
typedef std::vector<Pixel> Line;

struct Iterator
{
  using value_type = const Pixel;
  using reference  = const Pixel&;
  using pointer    = const Pixel*;

  reference operator*();
  Iterator& operator++();
  bool      operator==(const Iterator& that) const;

  std::vector<Pixel>::iterator pixel_iter;
  std::vector<Line>::iterator  line_iter;
};

class Image
{
public:
  Image(std::filesystem::path img_path);

  const Pixel& at(size_t line, size_t pixel) const;

  int get_width(void) const;
  int get_height(void) const;
  int get_n_channels(void) const;

  Iterator begin();
  Iterator end();

private:
  int               width;
  int               height;
  int               n_channels;
  std::vector<Line> img;
};

float
pixel_distance(const Pixel& p1, const Pixel& p2);
