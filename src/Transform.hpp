#pragma once

#include <array>

class Transform {
 public:
  std::array<double, 3> cm;  // Center of mass
  // std::array<std::array<double, 3>, 3> R;  // Orientation matrix
};