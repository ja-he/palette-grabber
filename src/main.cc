#include <iostream>

#include <cassert>

#include <algorithm>
#include <numeric>

#include "image.hh"

int
main(int argc, char* argv[])
{
  assert(argc > 1);

  Image img(argv[1]);
  printf("read in a %d by %d image with %d channels.\n",
         img.get_width(),
         img.get_height(),
         img.get_n_channels());

  float              distance_tolerance = 100.0f;
  std::vector<Pixel> distinct_colors;
  for (const auto& pixel : img) {
    bool new_color =
      std::all_of(distinct_colors.begin(),
                  distinct_colors.end(),
                  [&pixel, distance_tolerance](auto& color) {
                    return (pixel_distance(color, pixel) > distance_tolerance);
                  });
    if (new_color)
      distinct_colors.push_back(pixel);
  }

  std::cout << "Palette (" << distinct_colors.size()
            << " colors):" << std::endl;
  for (const auto& color : distinct_colors) {
    std::cout << "  " << color.to_hex() << std::endl;
  }

  return EXIT_SUCCESS;
}
