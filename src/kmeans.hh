#pragma once

#include <vector>

#include "p3d.hh"

std::vector<Point3D>
k_means(const std::vector<Point3D>& points, uint k, uint epochs);
