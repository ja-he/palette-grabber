#include <iostream>

#include <cassert>

#include <algorithm>
#include <numeric>

#define FMT_HEADER_ONLY
#include <fmt/core.h>

#include "cli.hh"
#include "image.hh"
#include "kmeans.hh"

void
run_k_means(const Image& img, uint k)
{
  fmt::print(
    stderr, "generating palette via k-means clustering\n  k = {}\n", k);

  auto                 colors = img.get_colors_vec();
  std::vector<Point3D> colors_vec;
  std::transform(colors.begin(),
                 colors.end(),
                 std::back_inserter(colors_vec),
                 [](auto& p) { return p.to_p3d(); });

  fmt::print(stderr, "  n = {}\n", colors.size());

  auto cluster_centroids = k_means(colors_vec, k, 10);
  for (const auto& point : cluster_centroids) {
    auto color = Pixel{
      (uint8_t)point.x,
      (uint8_t)point.y,
      (uint8_t)point.z,
    };
    std::cout << "  " << color.to_hex() << std::endl;
  }
}

int
main(int argc, char* argv[])
{
  Options opt(parse_args(argc, argv));

  Image img(opt.input_file);
  // printf("read in a %d by %d image with %d channels.\n",
  //        img.get_width(),
  //        img.get_height(),
  //        img.get_n_channels());

  if (opt.naive) {
    float distance_tolerance = opt.naive_threshold;
    assert(distance_tolerance >= 10);
    std::vector<Pixel> distinct_colors;
    for (const auto& pixel : img) {
      bool new_color = std::all_of(distinct_colors.begin(),
                                   distinct_colors.end(),
                                   [&pixel, distance_tolerance](auto& color) {
                                     return (pixel_distance(color, pixel) >
                                             distance_tolerance);
                                   });
      if (new_color)
        distinct_colors.push_back(pixel);
    }

    std::cout << "Palette (" << distinct_colors.size()
              << " colors):" << std::endl;
    for (const auto& color : distinct_colors) {
      std::cout << "  " << color.to_hex() << std::endl;
    }
  } else if (opt.dump_colors) {
    for (auto& color : img.get_colors_set()) {
      std::cout << (uint32_t)color.r << ' ' << (uint32_t)color.g << ' '
                << (uint32_t)color.b << ' ' << color.to_uint() << '\n';
    }
    std::cout << std::flush;
  } else if (opt.kmeans) {
    run_k_means(img, opt.kmeans);
  } else {
    uint default_k = 10;
    run_k_means(img, default_k);
  }

  return EXIT_SUCCESS;
}
