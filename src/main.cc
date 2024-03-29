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

  auto cluster_centroids = k_means(colors_vec, k, 100);
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

  if (opt.kmeans) {
    run_k_means(img, opt.kmeans);
  } else {
    const uint default_k = 10;
    run_k_means(img, default_k);
  }

  return EXIT_SUCCESS;
}
