#pragma once

#include <string>
#include <vector>

#include "Transform.hpp"

class Sphere {
 public:
  Transform tf;  // Sphere position and orientation

  double D = 1.;   // Diameter
  double R = 0.5;  // Radius

  Sphere() = default;

  Sphere(double diameter) {
    D = diameter;
    R = 0.5 * D;
  }

  double minDistance(const Sphere& np, const std::array<double, 3>& Lbox);
  bool overlapCheck(const Sphere& np, const std::array<double, 3>& Lbox);

  double minDistance(const std::vector<double> cm, const std::array<double, 3>& Lbox);

  std::string cogli1(std::string color, const std::array<double, 3>& Lbox);
};