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

  operator uint32_t() const;
};

bool
operator==(const Pixel& a, const Pixel& b);

bool
operator<(const Pixel& a, const Pixel& b);

typedef std::vector<Pixel> Line;

class Image
{
public:
  Image(std::filesystem::path img_path);

  const Pixel& at(size_t line, size_t pixel) const;

  int get_width(void) const;
  int get_height(void) const;
  int get_n_channels(void) const;

  std::set<Pixel>    get_colors_set(void) const;
  std::vector<Pixel> get_colors_vec(void) const;

  std::vector<Pixel>::const_iterator begin() const;
  std::vector<Pixel>::const_iterator end() const;

private:
  int                width;
  int                height;
  int                n_channels;
  std::vector<Pixel> img;
};

float
pixel_distance(const Pixel& p1, const Pixel& p2);
