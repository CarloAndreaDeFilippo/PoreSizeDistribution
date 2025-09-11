#pragma once

#include <array>
#include <memory>
#include <vector>

#include "Sphere.hpp"

class PoreSizeDistribution {
 public:
  std::array<double, 3> Lbox;
  std::vector<double> poreSizeDistr;
  std::vector<double> oldPoreSizeDistr;
  double binSize = 0.1;
  size_t binNumber = 1;

  std::vector<Sphere> poreSpheres;

  PoreSizeDistribution(const std::array<double, 3> lbox) {
    initialize(lbox);
  };

  void initialize(const std::array<double, 3>& lbox);
  void insertData(const double diameter);
  void insertSphere(const Sphere& sph);
  void normalizeHistogram(size_t numSteps);
  void saveHistogram(const std::string filename);
  double averageError();
  void cogliPore(const std::string filename, std::string color, const bool append);
};