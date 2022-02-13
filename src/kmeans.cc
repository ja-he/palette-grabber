#include "kmeans.hh"

#include <cassert>

#include <algorithm>
#include <numeric>
#include <random>

constexpr float MAX_3D_DIST = 441.7f; // ~= sqrt(3(255^2))

float
distance(const auto& a, const auto& b)
{
  return (std::sqrt(((a.x - b.x) * (a.x - b.x)) + ((a.y - b.y) * (a.y - b.y)) +
                    ((a.z - b.z) * (a.z - b.z))));
}

std::vector<Point3D>
k_means(const std::vector<Point3D>& input, uint k, uint epochs)
{
  auto points = input;

  std::default_random_engine rng(std::random_device{}());
  std::shuffle(points.begin(), points.end(), rng);

  assert(points.size() >= k);
  std::vector<std::pair<Point3D, std::vector<Point3D>>> centroids_with_points;
  std::for_each_n(points.begin(), k, [&centroids_with_points](Point3D p) {
    centroids_with_points.push_back({ p, {} });
  });

  for (uint epoch_counter = 0; epoch_counter < epochs; epoch_counter++) {

    // cluster points
    for (const auto& point : points) {
      auto   min_dist = MAX_3D_DIST;
      size_t min_idx  = 0;
      for (size_t i = 0; i < centroids_with_points.size(); i++) {
        const auto& [centroid, cluster] = centroids_with_points[i];

        auto cur_dist = distance(point, centroid);
        if (cur_dist < min_dist) {
          min_dist = cur_dist;
          min_idx  = i;
        }
      }

      centroids_with_points[min_idx].second.push_back(point);
    }

    // recompute centroids, clear cluster
    for (auto& [centroid, cluster] : centroids_with_points) {
      float num_points = cluster.size();

      auto new_centroid = std::accumulate(
        cluster.begin(),
        cluster.end(),
        Point3D{ 0.0, 0.0, 0.0 },
        [num_points](const Point3D& accumulator, const Point3D& p) {
          return Point3D{
            accumulator.x + (p.x / num_points),
            accumulator.y + (p.y / num_points),
            accumulator.z + (p.z / num_points),
          };
        });

      centroid = new_centroid;
      cluster = {};
    }
  }

  std::vector<Point3D> centroids;
  for (const auto& [centroid, _] : centroids_with_points) {
    centroids.push_back(centroid);
  }
  return centroids;
};
