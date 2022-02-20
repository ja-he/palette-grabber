#pragma once

struct Point3D
{
  float x = 0.0f;
  float y = 0.0f;
  float z = 0.0f;
};

float
distance(const Point3D& a, const Point3D& b);

Point3D
operator/(const Point3D& dividend, const float divisor);

Point3D
operator+(const Point3D& a, const Point3D& b);
