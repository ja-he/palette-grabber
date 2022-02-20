#include "p3d.hh"

#include <cmath>

float
distance(const Point3D& a, const Point3D& b)
{
  return (std::sqrt(((a.x - b.x) * (a.x - b.x)) + ((a.y - b.y) * (a.y - b.y)) +
                    ((a.z - b.z) * (a.z - b.z))));
}

Point3D
operator/(const Point3D& dividend, const float divisor)
{
  return Point3D{
    dividend.x / divisor,
    dividend.y / divisor,
    dividend.z / divisor,
  };
}

Point3D
operator+(const Point3D& a, const Point3D& b)
{
  return Point3D{
    a.x + b.x,
    a.y + b.y,
    a.z + b.z,
  };
}
