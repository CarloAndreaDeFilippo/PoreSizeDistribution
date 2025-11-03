#pragma once

#include <array>
#include <nlopt.hpp>
#include <vector>

#include "LinkedCellList.hpp"
#include "ParticleSystem.hpp"

struct ConstraintData {
  std::array<double, 3> vecP{0., 0., 0.};
  std::array<double, 3> Lbox = {1., 1., 1.};
};

double maxRadius(const std::vector<double>& x, std::vector<double>& grad, void* f_data);               // Function to maximize
double constraintIncludePoint(const std::vector<double>& x, std::vector<double>& grad, void* c_data);  // Function that describes the constraint

class Optimizer {
 public:
  nlopt::opt opt;
  ConstraintData constrData;

  //

  std::vector<double> x;
  std::vector<double> grad;
  double maxDist;

  Optimizer(std::array<double, 3> lbox) : opt(nlopt::LN_COBYLA, 4) {  // 4D: x, y, z, r
    constrData.Lbox = lbox;
  }

  void randomPoint(const ParticleSystem& partSys, const LinkedCellList<Sphere>& cellList);
  void initializeOptimization(ParticleSystem& partSys);
  void optimize();
};