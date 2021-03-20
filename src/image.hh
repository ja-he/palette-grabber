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
};
typedef std::vector<Pixel> Line;

struct Iterator
{
  using value_type = const Pixel;
  using reference  = const Pixel&;
  using pointer    = const Pixel*;

  reference operator*() { return *pixel_iter; }

  Iterator& operator++()
  {
    if (++pixel_iter == line_iter->end()) { line_iter++; pixel_iter = line_iter->begin(); }
    return *this;
  }

  bool operator==(const Iterator& that) const
  {
    return line_iter == that.line_iter && pixel_iter == that.pixel_iter;
  }

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

  Iterator begin()
  {
    return Iterator{ this->img.begin()->begin(), this->img.begin() };
  }
  Iterator end() { return Iterator{ this->img.end()->begin(), this->img.end() }; }

private:
  int               width;
  int               height;
  int               n_channels;
  std::vector<Line> img;
};

float
pixel_distance(const Pixel& p1, const Pixel& p2);
