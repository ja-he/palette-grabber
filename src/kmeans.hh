#pragma once

#include <cassert>

#include <random>
#include <vector>

#include <algorithm>
#include <numeric>

#include <limits>

#include <range/v3/all.hpp>

constexpr float KMEANS_MAX_DISTANCE = std::numeric_limits<float>::max();

template<typename T>
std::vector<T>
k_means(const std::vector<T>& points, uint k, uint epochs);

template<typename T>
std::vector<T>
k_means(const std::vector<T>& input, uint k, uint epochs)
{
  auto points = input;

  std::default_random_engine rng(std::random_device{}());
  std::shuffle(points.begin(), points.end(), rng);

  assert(points.size() >= k);
  std::vector<std::pair<T, std::vector<T>>> centroids_with_points;
  std::for_each_n(points.begin(), k, [&centroids_with_points](T p) {
    centroids_with_points.push_back({ p, {} });
  });

  std::vector<T> prev_centroids(k, { 0.0f, 0.0f, 0.0f });

  for (uint epoch_counter = 0; epoch_counter < epochs; epoch_counter++) {

    // cluster points
    for (const auto& point : points) {
      auto   min_dist = KMEANS_MAX_DISTANCE;
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

      auto new_centroid =
        std::accumulate(cluster.begin(),
                        cluster.end(),
                        T{ 0.0 },
                        [num_points](const T& accumulator, const T& p) {
                          return accumulator + (p / num_points);
                        });

      centroid = new_centroid;
      cluster  = {};
    }

    // stop if centroids hardly changed
    if (total_centroid_shift(prev_centroids, centroids_with_points) < 0.01f) {
      break;
    }

    // set previous centroids
    for (size_t i = 0; i < centroids_with_points.size(); i++) {
      prev_centroids[i] = centroids_with_points[i].first;
    }
  }

  std::vector<T> centroids;
  for (const auto& [centroid, _] : centroids_with_points) {
    centroids.push_back(centroid);
  }
  return centroids;
};

template<typename T>
float
total_centroid_shift(
  const std::vector<T>&                            centroids_prev,
  const std::vector<std::pair<T, std::vector<T>>>& centroids_with_cluster)
{
  // clang-format off
  auto old_and_new_centroid_pairs =
    ranges::views::zip(centroids_prev,
                       centroids_with_cluster
                       | ranges::views::transform([](auto& pair) {
                           return pair.first;
                         }))
    | ranges::to<std::vector<std::pair<T, T>>>;
  // clang-format on

  return ranges::accumulate(
    old_and_new_centroid_pairs,
    0.0f,
    [](float accumulator, const auto& centroid_pair) {
      return accumulator + distance(centroid_pair.first, centroid_pair.second);
    });
}
