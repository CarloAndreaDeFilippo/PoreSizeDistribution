#pragma once

#include <string>
#include <vector>

#include "Sphere.hpp"

class ParticleSystem {
 public:
  size_t N = 0;  // Number of particles
  std::vector<Sphere> particles;

  std::array<double, 3> Lbox = {1., 1., 1.};

  ParticleSystem() = default;
  ParticleSystem(const std::string infile) {
    loadSystem(infile);
  }

  void addParticle(const Sphere& part);
  void loadSystem(const std::string infile);

  void cogli2(const std::string filename, std::string color, const bool append);
};