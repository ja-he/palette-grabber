#include "image.hh"

#include <stdint.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include <iomanip>
#include <sstream>
#include <string>

#include <cmath>

Image::Image(std::filesystem::path img_path)
{
  // TODO(ja-he): verify file existence, permissions, ... neatly

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
}

const Pixel&
Image::at(size_t line, size_t pixel) const
{
  return this->img[line][pixel];
}

int
Image::get_width(void) const
{
  return this->width;
}
int
Image::get_height(void) const
{
  return this->height;
}
int
Image::get_n_channels(void) const
{
  return this->n_channels;
}

Point3D
Pixel::to_p3d(void) const
{
  return {
    (float)this->r,
    (float)this->g,
    (float)this->b,
  };
}

uint32_t
Pixel::to_uint(void) const
{
  return ((r << 16) | (g << 8) | b);
}

std::string
Pixel::to_hex(void) const
{
  std::stringstream ss;
  ss << '#';
  ss << std::hex;
  ss << std::setw(2) << std::setfill('0') << (uint32_t)(this->r);
  ss << std::setw(2) << std::setfill('0') << (uint32_t)(this->g);
  ss << std::setw(2) << std::setfill('0') << (uint32_t)(this->b);

  return ss.str();
}

float
pixel_distance(const Pixel& p1, const Pixel& p2)
{
  return std::sqrt(((p1.r - p2.r) * (p1.r - p2.r)) +
                   ((p1.g - p2.g) * (p1.g - p2.g)) +
                   ((p1.b - p2.b) * (p1.b - p2.b)));
}

Iterator
Image::begin()
{
  return Iterator{ this->img.begin()->begin(),
                   this->img.begin(),
                   this->img.end() };
}

Iterator
Image::end()
{
  return Iterator{ this->img.end(), this->img.end() };
}

Iterator::reference
Iterator::operator*()
{
  return *pixel_iter;
}

Iterator&
Iterator::operator++()
{
  assert(this->line_iter_usable);
  if (++pixel_iter == line_iter->end()) {
    line_iter++;
    if (line_iter != line_end) {
      pixel_iter = line_iter->begin();
    }
  }
  return *this;
}

bool
Iterator::operator==(const Iterator& that) const
{
  if (this->line_iter == this->line_end) {
    assert(that.line_iter == that.line_end);
    return true;
  }
  return line_iter == that.line_iter && pixel_iter == that.pixel_iter;
}

std::set<Pixel>
Image::get_colors_set(void) const
{
  std::set<Pixel> colors;
  for (auto& line : img) {
    for (auto& color : line) {
      colors.insert(color);
    }
  }
  return colors;
}

std::vector<Pixel>
Image::get_colors_vec(void) const
{
  std::vector<Pixel> colors;
  for (auto& line : img) {
    for (auto& color : line) {
      colors.push_back(color);
    }
  }
  std::sort(colors.begin(), colors.end());
  colors.erase(std::unique(colors.begin(), colors.end()), colors.end());

  return colors;
}

bool
operator<(const Pixel& a, const Pixel& b)
{
  uint32_t an = (a.r << 16) | (a.g << 8) | (a.b);
  uint32_t bn = (b.r << 16) | (b.g << 8) | (b.b);
  return an < bn;
}

bool
operator==(const Pixel& a, const Pixel& b)
{
  return (a.r == b.r && a.g == b.g && a.b == b.b);
}
