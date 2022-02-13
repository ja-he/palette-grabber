#pragma once

#include "p3d.hh"
#include <filesystem>
#include <iostream>

#include <set>
#include <string>
#include <vector>

struct Pixel
{
  uint8_t r = 0;
  uint8_t g = 0;
  uint8_t b = 0;

  std::string to_hex(void) const;
  uint32_t    to_uint(void) const;
  Point3D     to_p3d(void) const;
};

bool
operator<(const Pixel& a, const Pixel& b);

typedef std::vector<Pixel> Line;

struct Iterator
{
  using value_type = const Pixel;
  using reference  = const Pixel&;
  using pointer    = const Pixel*;

  reference operator*();
  Iterator& operator++();
  bool      operator==(const Iterator& that) const;

  Iterator(std::vector<Pixel>::iterator pixel_iter,
           std::vector<Line>::iterator  line_iter,
           std::vector<Line>::iterator  line_end)
    : line_iter_usable(true)
    , pixel_iter(pixel_iter)
    , line_iter(line_iter)
    , line_end(line_end){};
  Iterator(std::vector<Line>::iterator line_iter,
           std::vector<Line>::iterator line_end)
    : line_iter(line_iter)
    , line_end(line_end){};

private:
  bool                              line_iter_usable = false;
  std::vector<Pixel>::iterator      pixel_iter;
  std::vector<Line>::iterator       line_iter;
  const std::vector<Line>::iterator line_end;
};

class Image
{
public:
  Image(std::filesystem::path img_path);

  const Pixel& at(size_t line, size_t pixel) const;

  int get_width(void) const;
  int get_height(void) const;
  int get_n_channels(void) const;

  std::set<Pixel> get_colors(void) const;

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
